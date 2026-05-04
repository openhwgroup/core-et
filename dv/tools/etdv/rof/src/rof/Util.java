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

import java.util.function.Consumer;

import static gblibx.MessageManager.error;
import static gblibx.Util.*;

public class Util {
    public static <R> R expectValue(JSONObject jobj, String key, Class clz) {
        if (jobj.has(key)) {
            //return gblibx.Util.<R>castobj(toPOJO(jobj.get(key)));
            final String expectType = clz.getSimpleName();
            Object pojo = toPOJO(jobj.get(key));
            if (expectType.equals("String[]") && Object[].class.isInstance(pojo)) {
                Object[] pojos = castobj(pojo);
                if (1 > pojos.length) return castobj(new String[0]);
                return castobj(toArrayOfT(pojos));
            } else if (clz.isInstance(pojo)) {
                return castobj(pojo);
            }
            errorAndExit("TYPE-1", pojo.getClass().getSimpleName(), key, expectType);
        } else {
            errorAndExit("KEY-1", key);
        }
        return null;
    }

    public static void errorAndExit(String key, Object... vals) {
        error(key, vals);
        System.exit(1);
    }

    public static String[] splitName(String name) {
        return name.split("\\.");
    }

    public static <T> String getTypeName(T e) {
        return e.getClass().getSimpleName().toLowerCase();
    }

}
