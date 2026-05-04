/*
 *
 *  * The MIT License
 *  *
 *  * Copyright 2020 kpfalzer.
 *  *
 *  * Permission is hereby granted, free of charge, to any person obtaining a copy
 *  * of this software and associated documentation files (the "Software"), to deal
 *  * in the Software without restriction, including without limitation the rights
 *  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  * copies of the Software, and to permit persons to whom the Software is
 *  * furnished to do so, subject to the following conditions:
 *  *
 *  * The above copyright notice and this permission notice shall be included in
 *  * all copies or substantial portions of the Software.
 *  *
 *  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  * THE SOFTWARE.
 *
 *
 */

package rof;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.*;

import static gblibx.MessageManager.error;
import static gblibx.MessageManager.info;
import static gblibx.Util.*;
import static rof.Util.*;

public class Main {
    public static void main(String[] argv) {
        if (1 != argv.length) __usageError();
        try {
            (new Main(argv)).__process();
        } catch (Exception | IOException ex) {
            System.err.println(ex.getMessage());
            ex.printStackTrace(System.err);
            System.exit(1);
        }
        System.exit(0);
    }

    private static void __usageError() {
        System.err.println("\nUsage: rof rof.json\n");
        System.exit(1);
    }

    private Main(String[] argv) throws IOException {
        __argv = argv;
        __log = new MessageManager("rof.log");
    }

    private void __process() {
        __config = new Config(__argv[0]);
        boolean ok = __config.load();
        invariant(ok);
        __querySignatures()
                .__computeTests()
                .__generate();
    }

    private Main __querySignatures() {
        return __filterSignatures(Signature.query(__config.runroot()));
    }

    private Main __filterSignatures(Signature[] signatures) {
        for (Signature sig : signatures) {
            if (__config.exclude(sig.signature)) {
                info("SIG-1", sig.signature);
            } else {
                __signatures.add(sig);
            }
        }
        return this;
    }

    private Main __computeTests() {
        Map<String, Build> builds = new HashMap<>();
        Map<String, Test> tests = new HashMap<>();
        for (Build b : __config.getBuilds()) {
            String name = castobj(b.get("full_name"));
            builds.put(name, b);
        }
        for (Test t : __config.getTests()) {
            String name = castobj(t.get("full_name"));
            tests.put(name, t);
        }
        for (Signature sig : __signatures) {
            int testCnt = __config.getTestCnt(sig.signature);
            Signature.Raw[] raws = sig.matches;
            if (testCnt > raws.length) {
                testCnt = raws.length;
            }
            for (int i = 0; i < testCnt; i++) {
                __addTest(builds, tests, raws[i], sig.signature);
            }
        }
        return this;
    }

    private void __addTest(Map<String, Build> builds,
                           Map<String, Test> tests,
                           Signature.Raw candidate,
                           String signature) {
        final String testFullName = candidate.getFullName();
        final Test found = tests.get(testFullName);
        if (isNonNull(found)) {
            final String buildName = candidate.getBuildName();
            boolean buildOK = true;
            if (!__builds.containsKey(buildName)) {
                final Build b = builds.get(buildName);
                if (isNonNull(b)) {
                    __builds.put(buildName, b);
                } else {
                    error("BUILD-1", buildName);
                    buildOK = false;
                }
            }
            if (buildOK) {
                info("SIG-2", testFullName, signature);
                __tests.add(found);
            } else {
                error("TEST-2", testFullName);
            }
        } else {
            error("TEST-1", testFullName);
        }
    }

    private Main __generate() {
        File dir = new File(__config.runroot(), "rof");
        if (!dir.isDirectory()) {
            errorAndExit("DIR-1", dir);
        }
        return __generateConfigPy(dir).__generateRunRof(dir);
    }

    private static final String __SPLITX = "@SX@";
    private static final String __SPLITX_REPL = " \\\n  ";

    private static final String __RUNCMD =
            "et-dvrun" + __SPLITX
                    + "--config ./rof.etdv.py" + __SPLITX
                    + "--runroot ./dvrun" + __SPLITX
                    + "--make" + __SPLITX;

    private static final String[] __KEEP_OPTS = {"-i", "-k", "--noclean", "-s", "--status"};

    private String[] __cargv() {
        return __config.cmdLine();
    }

    private void __addWithOpt(StringBuilder cmd, String opt) {
        int i = find(__cargv(), opt);
        if (0 <= i) {
            cmd.append(opt).append(' ').append(__cargv()[i + 1]).append(__SPLITX);
        }
    }

    private Main __generateRunRof(File dir) {
        File outf = new File(dir, "run_dvrun");
        info("RUN-1", outf);
        StringBuilder cmd = new StringBuilder(__RUNCMD);
        if (contains(__cargv(), "--noslurm")) {
            cmd.append("--noslurm").append(__SPLITX);
        } else {
            int i = find(__cargv(), "--slurmopts");
            String slurmopts = (0 <= i) ? __cargv()[i + 1] : "";
            cmd.append("--slurm").append(__SPLITX)
                    .append("--slurmopts \"").append(slurmopts).append(" --nice\"").append(__SPLITX);
        }
        __addWithOpt(cmd, "--timeout");
        __addWithOpt(cmd, "-j");
        __addWithOpt(cmd, "--verbose");
        for (String k : __KEEP_OPTS) {
            if (contains(__cargv(), k)) {
                cmd.append(k).append(__SPLITX);
            }
        }
        try (PrintWriter os = new PrintWriter(outf)) {
            os.println("#!/bin/csh -f\n");
            __header(os);
            os.println("/bin/rm -rf ./dvrun");
            os.println("/usr/bin/egrep -q 'config_name.+unknown' ./rof.etdv.py");
            os.println(
                    "if (\"0\" == \"${status}\") then\n"
                            + "    echo 'No signatures, rof does nothing'\n"
                            + "    exit 0\n"
                            + "endif"
            );
            String xcmd = cmd.toString().replace(__SPLITX, __SPLITX_REPL);
            os.println(xcmd.substring(0, xcmd.length() - __SPLITX_REPL.length()));
        } catch (FileNotFoundException e) {
            errorAndExit("FILE-3", outf, e.getMessage());
        }
        try {
            setFilePermissions(outf, "rwxr-xr-x");
        } catch (IOException e) {
            errorAndExit("FILE-3", outf, e.getMessage());
        }
        return this;
    }

    private Main __generateConfigPy(File dir) {
        File outf = new File(dir, "rof.etdv.py");
        info("CONFIG-1", outf);
        String configName = (__tests.isEmpty()) ? "unknown" : __tests.get(0).getConfigName();
        try (PrintWriter os = new PrintWriter(outf)) {
            __header(os).printf("config_name = '%s'\n\n", configName);
            __dumpBuilds(os).__dumpTests(os);
        } catch (FileNotFoundException e) {
            errorAndExit("FILE-3", outf, e.getMessage());
        }
        return this;
    }

    private static final String[] __ATTRS = new String[]{"run_cmd", "check_cmd"};
    private static String __3QUOTES = "\"\"\"";

    private Main __dumpBuilds(PrintWriter os) {
        os.println("build_by_name = {}");
        for (Map.Entry<String, Build> e : __builds.entrySet()) {
            final Build build = e.getValue();
            final String name = __dump(os, build);
            os.printf("build_by_name['%s'] = build\n", name);
            os.println("add_build(build)");
        }
        return this;
    }

    private <T extends Element> String __dump(PrintWriter os, T e) {
        final String type = getTypeName(e);
        final String name = castobj(e.get("name"));
        os.printf("\n%s = new_%s(name='%s')\n", type, type, name);
        for (String k : __ATTRS) {
            final String val = castobj(e.get("rof_" + k));
            final String xtra = (k.startsWith("run"))
                    ? String.format("export ETDV_ORIG_RUNDIR=%s\n", __getOrigRunDir(e))
                    : "";
            os.printf("val = %s\n%s%s\n%s\n", __3QUOTES, xtra, val, __3QUOTES);
            os.printf("%s['%s'] = val\n", type, k);
        }
        return name;
    }

    private Main __dumpTests(PrintWriter os) {
        for (Test test : __tests) {
            __dump(os, test);
            final String buildName = test.getBuildName();
            os.printf("build_by_name['%s'].add_test(test);\n", buildName);
        }
        return this;
    }

    private <T extends Element> String __getOrigRunDir(T e) {
        String type = getTypeName(e);
        String names[] = splitName(e.getFullName());
        String tail = null;
        if (type.equalsIgnoreCase("build")) {
            tail = String.format("makefile/%s/%s/build", names[0], names[1]);
        } else {
            tail = String.format("makefile/%s/%s/tests/%s", names[0], names[1], names[2]);
        }
        return getAbsoluteFileName(new File(__config.runroot(), tail));
    }

    private static PrintWriter __header(PrintWriter os) {
        os.printf("# Generated by rof\n# on %s\n\n", getLocalDateTime());
        return os;
    }

    private final String[] __argv;
    private Config __config;
    private final MessageManager __log;
    private List<Signature> __signatures = new LinkedList<>();
    private Map<String, Build> __builds = new HashMap<>();
    private List<Test> __tests = new LinkedList<>();
}
