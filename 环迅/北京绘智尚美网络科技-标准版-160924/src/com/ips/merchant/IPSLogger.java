package com.ips.merchant;

import java.util.Hashtable;

import android.util.Log;

/**
 * The class for print log
 * 
 * @author zhangll
 *
 */
public class IPSLogger {
    private final static boolean isLoggable = true;

    public final static String appName = "[IPS_UPMP]";
    private final static int logLevel = Log.VERBOSE;
    private static Hashtable<String, IPSLogger> sLoggerTable = new Hashtable<String, IPSLogger>();
    private String mClassName;

    private static IPSLogger ipsLog;
    private static final String IPS = "@IPS@ ";

    private IPSLogger(String name) {
        mClassName = name;
    }

    /**
     * 
     * @param className
     * @return
     */
    @SuppressWarnings("unused")
    private static IPSLogger getLogger(String className) {
        IPSLogger classLogger = (IPSLogger) sLoggerTable.get(className);
        if (classLogger == null) {
            classLogger = new IPSLogger(className);
            sLoggerTable.put(className, classLogger);
        }
        return classLogger;
    }

    /**
     * Purpose:Mark user two
     * 
     * @return
     */
    public static IPSLogger ipsLog() {
        if (ipsLog == null) {
            ipsLog = new IPSLogger(IPS);
        }
        return ipsLog;
    }

    /**
     * Get The Current Function Name
     * 
     * @return
     */
    private String getFunctionName() {
        StackTraceElement[] sts = Thread.currentThread().getStackTrace();
        if (sts == null) {
            return null;
        }
        for (StackTraceElement st : sts) {
            if (st.isNativeMethod()) {
                continue;
            }
            if (st.getClassName().equals(Thread.class.getName())) {
                continue;
            }
            if (st.getClassName().equals(this.getClass().getName())) {
                continue;
            }
            // return mClassName + "[ " + Thread.currentThread().getName() +
            // ": "
            // + st.getFileName() + ":" + st.getLineNumber() + " "
            // + st.getMethodName() + " ]";

            return new StringBuilder().append("[").append(st.getMethodName()).append(": ").append(st.getLineNumber()).append(":").append(mClassName)
                    .append(":").append(st.getFileName()).append(":").append(Thread.currentThread().getName()).append("]").toString();
        }
        return null;
    }

    /**
     * The Log Level:i
     * 
     * @param str
     */
    public void i(Object str) {
        if (isLoggable) {
            if (logLevel <= Log.INFO) {
                String name = getFunctionName();
                if (name != null) {
                    Log.i(name, str.toString());
                } else {
                    Log.i(appName, str.toString());
                }
            }
        }
    }

    /**
     * The Log Level:d
     * 
     * @param str
     */
    public void d(Object str) {
        if (isLoggable) {
            if (logLevel <= Log.DEBUG) {
                String name = getFunctionName();
                if (name != null) {
                    Log.d(name, str.toString());
                } else {
                    Log.d(appName, str.toString());
                }
            }
        }
    }

    /**
     * The Log Level:V
     * 
     * @param str
     */
    public void v(Object str) {
        if (isLoggable) {
            if (logLevel <= Log.VERBOSE) {
                String name = getFunctionName();
                if (name != null) {
                    Log.v(name, str.toString());
                } else {
                    Log.v(appName, str.toString());
                }
            }
        }
    }

    /**
     * The Log Level:w
     * 
     * @param str
     */
    public void w(Object str) {
        if (isLoggable) {
            if (logLevel <= Log.WARN) {
                String name = getFunctionName();
                if (name != null) {
                    Log.w(name, str.toString());
                } else {
                    Log.w(appName, str.toString());
                }
            }
        }
    }

    /**
     * The Log Level:e
     * 
     * @param str
     */
    public void e(Object str) {
        if (isLoggable) {
            if (logLevel <= Log.ERROR) {
                String name = getFunctionName();
                if (name != null) {
                    Log.e(name, str.toString());
                } else {
                    Log.e(appName, str.toString());
                }
            }
        }
    }

    /**
     * The Log Level:e
     * 
     * @param ex
     */
    public void e(Exception ex) {
        if (isLoggable) {
            if (logLevel <= Log.ERROR) {
                Log.e(appName, "error", ex);
            }
        }
    }

    /**
     * The Log Level:e
     * 
     * @param log
     * @param tr
     */
    public void e(String log, Throwable tr) {
        if (isLoggable) {
            String line = getFunctionName();
            Log.e(appName, "{Thread:" + Thread.currentThread().getName() + "}" + "[" + mClassName + line + ":] " + log + "\n", tr);
        }
    }
}