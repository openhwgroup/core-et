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

import gblibx.HttpConnection;

import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;

import static gblibx.MessageManager.error;
import static gblibx.MessageManager.info;
import static gblibx.Util.*;
import static rof.Util.errorAndExit;

public class Signature {
    public static Signature[] query(String runroot) {
        int runid = __getRunId(runroot);
        invariant(0 < runid);
        Map<String, Object> data = null;
        try {
            data = HttpConnection.get(__HOST, __PORT, __PATH, "run_id="+runid);
        } catch (HttpConnection.Exception e) {
            errorAndExit("URL-1", __HOST, __PORT, __PATH);
        }
        invariant(data.containsKey("data"));
        return __parse(castobj(data.get("data")));
    }

    private static Signature[] __parse(Object[] eles) {
        Signature[] sigs = new Signature[eles.length];
        for (int i = 0; i < sigs.length; i++) {
            sigs[i] = new Signature(castobj(eles[i]));
        }
        return sigs;
    }

    public final String signature;
    public final Raw[] matches;

    private Signature(Object[] sig) {
        invariant(2 == sig.length);
        signature = castobj(sig[0]);
        Object[] matches = castobj(sig[1]);
        this.matches = new Raw[matches.length];
        for (int i = 0; i < matches.length; i++) {
            this.matches[i] = new Raw(castobj(matches[i]));
        }
        __sortMatchesByCpu();
    }

    /**
     * Sort in ascending order by cputime.
     */
    private void __sortMatchesByCpu() {
        Arrays.sort(matches, new Comparator<Raw>() {
            @Override
            public int compare(Raw o1, Raw o2) {
                final double c1 = o1.cputime(), c2 = o2.cputime();
                return (c1 < c2)
                        ? -1
                        : ((c1 == c2) ? 0 : 1);
            }
        });
    }

    public static class Raw extends HashMap<String,Object> {
        private Raw(HashMap<String,Object> m) {
            super.putAll(m);
        }

        public String getFullName() {
            return String.format("%s.%s", getBuildName(), super.get("name"));
        }

        public String getBuildName() {
            return castobj(super.get("config"));
        }

        public double cputime() {
            return castobj(super.get("cputime"));
        }
    }

    private static int __getRunId(String runroot) {
        File f = new File(runroot, "makefile/.run_id");
        if (!f.exists()) {
            errorAndExit("RUNID-1", f);
        }
        try {
            info("FILE-2", f);
            return Integer.parseInt(readFile(f.getPath()));
        } catch (NumberFormatException | IOException e) {
            errorAndExit("FILE-1", f);
        }
        expectNever();
        return -1;
    }

    private static final String __HOST = System.getProperty(
            "sigurl.host",
            "localhost");
    private static final Integer __PORT = Integer.parseInt(System.getProperty(
            "sigurl.port",
            "3004"));
    private static final String __PATH = System.getProperty(
            "sigurl.path",
            "/dv_metrix/signature/summary_table.json");
}
