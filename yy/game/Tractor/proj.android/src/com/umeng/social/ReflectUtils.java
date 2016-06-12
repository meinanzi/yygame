/**
 *
 *	created by Mr.Simple, Nov 6, 201412:40:49 PM.
 *	Copyright (c) 2014, hehonghui@umeng.com All Rights Reserved.
 *
 *                #####################################################
 *                #                                                   #
 *                #                       _oo0oo_                     #   
 *                #                      o8888888o                    #
 *                #                      88" . "88                    #
 *                #                      (| -_- |)                    #
 *                #                      0\  =  /0                    #   
 *                #                    ___/`---'\___                  #
 *                #                  .' \\|     |# '.                 #
 *                #                 / \\|||  :  |||# \                #
 *                #                / _||||| -:- |||||- \              #
 *                #               |   | \\\  -  #/ |   |              #
 *                #               | \_|  ''\---/''  |_/ |             #
 *                #               \  .-\__  '-'  ___/-. /             #
 *                #             ___'. .'  /--.--\  `. .'___           #
 *                #          ."" '<  `.___\_<|>_/___.' >' "".         #
 *                #         | | :  `- \`.;`\ _ /`;.`/ - ` : | |       #
 *                #         \  \ `_.   \_ __\ /__ _/   .-` /  /       #
 *                #     =====`-.____`.___ \_____/___.-`___.-'=====    #
 *                #                       `=---='                     #
 *                #     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   #
 *                #                                                   #
 *                #               佛祖保佑         永无BUG              #
 *                #                                                   #
 *                #####################################################
 */

package com.umeng.social;

import android.text.TextUtils;
import android.util.Log;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

/**
 * @author mrsimple
 */
public final class ReflectUtils {
    /**
     * 根据方法名调用相应的方法</br>
     * 
     * @param value 调用方法的实例
     * @param methodName 方法名
     * @param parameterTypes 方法的参数类型
     * @param args 方法的参数值
     */
    public static void invokeMethod(Object object, String methodName, Class<?>[] parameterTypes,
            Object[] args) {
        if (object == null || TextUtils.isEmpty(methodName)) {
            return;
        }
        try {
            Method method = object.getClass().getMethod(methodName, parameterTypes);
            Log.d("", "### 找到 " + methodName + ", obj : " + object);
            method.invoke(object, args);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
    }

    /**
     * 创建clzpath对应类的实例</br>
     * 
     * @param clzPath 类路径
     * @param parameterTypes 该类构造函数的参数类型
     * @param args 该类的构造函数的参数
     * @return 该clzPath对应类的实例，如果该类不存在则返回null
     */
    public static Object newHandlerInstance(String clzPath, Class<?>[] parameterTypes, Object[] args) {
        try {
            Class<?> clz = Class.forName(clzPath);
            Constructor<?> constructor = clz.getConstructor(parameterTypes);
            return constructor.newInstance(args);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        return null;
    }
}
