package com.hn.market.core;


import java.util.LinkedList;
import java.util.NoSuchElementException;

import android.util.Log;

public class AyncTaskManager extends Thread {
	private static final String TAG = AyncTaskManager.class.getSimpleName();
	private static AyncTaskManager mAyncTaskManager = null;
	private boolean mIsStop = false;
	private LinkedList<AyncTask> taskList = new LinkedList<AyncTask>();

	public static AyncTaskManager SharedAyncTaskManager() {
		if (mAyncTaskManager == null) {
			synchronized (AyncTaskManager.class) {
				if (mAyncTaskManager == null) {
					mAyncTaskManager = new AyncTaskManager();
					mAyncTaskManager.start();
				}	
			}
		}
		return mAyncTaskManager;
	}
	
	private AyncTaskManager() {
	}

	public void addTask(AyncTask task) {
		if (task == null) {
			throw new NullPointerException("task is null");
		}
		synchronized (taskList) {
			taskList.add(task);
			taskList.notify();
		}
	}

	public void stopAllTask() {
		synchronized (taskList) {
			this.mIsStop = true;
			taskList.notify();
		}
	}

	@Override
	public void run() {
		while (!mIsStop) {
			AyncTask task = null;
			synchronized (taskList) {
				try {
					if (taskList.size() == 0 && !mIsStop) {
						taskList.wait();
					}
					if (taskList.size() > 0) {
						task = taskList.removeFirst();
						if (task != null) {
							task.executeMethod();
						}
					}
				} catch (InterruptedException e) {
					Log.e(TAG, "error:" + e.getMessage());
				} catch (NoSuchElementException e) {
					Log.e(TAG, "error:" + e.getMessage());
				}
			}
		}
	}
}

