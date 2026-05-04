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

import gblibx.MultiLogger;

import java.io.IOException;

import static gblibx.Util.getAbsoluteFileName;

public class MessageManager extends gblibx.MessageManager {
    public MessageManager(String logFname) throws IOException {
        super(new MultiLogger(logFname));
        info("LOG-1", getAbsoluteFileName(logFname));
    }

    static {
        _add("BUILD-1", "%s: could not find build in rof.json");
        _add("CONFIG-1", "%s: generating config file...");
        _add("DIR-1", "%s: directory does not exist");
        _add("FILE-1", "%s: error processing file");
        _add("FILE-2", "%s: processing file");
        _add("FILE-3", "%s: error writing file: %s");
        _add("FILE-4", "%s: cannot read file");
        _add("JSON-1", "%s: processing config JSON file ...");
        _add("KEY-1", "%s: no value found for key (in JSON file)");
        _add("LOG-1", "%s: creating logfile ...");
        _add("REX-1", "%s: invalid regular expression in %s setting (ignored)");
        _add("RUN-1", "%s: generating run file...");
        _add("RUNID-1", "%s: file does not exist");
        _add("SIG-1", "%s: signature excluded (due to rof_signature_exclude[])");
        _add("SIG-2", "%s: add test for signature '%s'");
        _add("TEST-1", "%s: could not find test in rof.json");
        _add("TEST-2", "%s: test excluded since no build found");
        _add("TYPE-1", "found type %s for key '%s', expected %s");
        _add("URL-1", "Invalid URL: http://%s:%d%s");
    }
}