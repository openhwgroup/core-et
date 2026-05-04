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

import org.json.JSONObject;

import java.io.IOException;
import java.lang.reflect.Array;
import java.nio.file.Files;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.function.Function;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;

import static gblibx.MessageManager.info;
import static gblibx.MessageManager.warning;
import static gblibx.Util.*;
import static rof.Util.errorAndExit;
import static rof.Util.expectValue;

public class Config {
    public Config(String jsonFname) {
        __jsonFileName = jsonFname;
        if (!fileIsReadable(__jsonFileName)) {
            errorAndExit("FILE-4", __jsonFileName);
        }
    }

    public boolean load() {
        boolean ok = false;
        try {
            info("JSON-1", getAbsoluteFileName(__jsonFileName));
            final JSONObject jsonConfig = readJSONObject(__jsonFileName);
            ok = __process(jsonConfig);
        } catch (IOException e) {
            throw new Exception(e);
        }
        return ok;
    }

    public boolean __process(JSONObject jobj) {
        boolean ok = true;
        __runroot = expectValue(jobj, "runroot", String.class);
        __cmdLine = expectValue(jobj, "cmdline", String[].class);
        __createExclude(expectValue(jobj, "rof_signature_exclude", String[].class));
        __createCntBySignature(expectValue(jobj, "rof_testcnt_by_signature", Map.class));
        __builds = __toElement(jobj, "builds", e -> new Build(e));
        __tests = __toElement(jobj, "tests", e -> new Test(e));
        return ok;
    }

    private void __createExclude(String[] rex) {
        List<Pattern> eles = new LinkedList<>();
        for (String s : rex) {
            try {
                Pattern p = Pattern.compile(s, Pattern.CASE_INSENSITIVE);
                eles.add(p);
            } catch (PatternSyntaxException ex) {
                warning("REX-1", s, "rof_signature_exclude");
            }
        }
        __signatureExclude = eles.toArray(new Pattern[0]);
    }

    private void __createCntBySignature(Map<String, Integer> eles) {
        eles.forEach((k, v) -> {
            try {
                Pattern p = Pattern.compile(k, Pattern.CASE_INSENSITIVE);
                __testCntBySignature.put(p, v);
            } catch (PatternSyntaxException ex) {
                warning("REX-1", k, "rof_testcnt_by_signature");
            }
        });
    }

    private static <R extends Element> R[] __toElement(JSONObject jobj, String key, Function<String, R> creator) {
        Object[] objs = expectValue(jobj, key, Object[].class);
        Class clz = (key.equals("builds")) ? Build.class : Test.class;
        R[] eles = castobj(Array.newInstance(clz, objs.length));
        for (int i = 0; i < objs.length; i++) {
            Map<String, Object> x = castobj(objs[i]);
            eles[i] = creator.apply(castobj(x.get("full_name")));
            eles[i].putAll(x);
        }
        return eles;
    }

    public String runroot() {
        return __runroot;
    }

    public boolean exclude(String signature) {
        for (Pattern p : __signatureExclude) {
            if (p.matcher(signature).find()) {
                return true;
            }
        }
        return false;
    }

    public String[] cmdLine() {
        return __cmdLine;
    }

    public int getTestCnt(String signature) {
        for (Map.Entry<Pattern, Integer> kv : __testCntBySignature.entrySet()) {
            if (kv.getKey().matcher(signature).find()) {
                return kv.getValue();
            }
        }
        return 1;
    }

    public Build[] getBuilds() {
        return __builds;
    }

    public Test[] getTests() {
        return __tests;
    }

    private final String __jsonFileName;
    private String __runroot;
    private String[] __cmdLine;
    private Pattern[] __signatureExclude;
    private Map<Pattern, Integer> __testCntBySignature = new HashMap<>();
    private Build[] __builds;
    private Test[] __tests;
}
