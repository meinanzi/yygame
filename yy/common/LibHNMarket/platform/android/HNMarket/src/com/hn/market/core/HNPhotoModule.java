//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved

package com.hn.market.core;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Base64;
import android.util.Log;
import com.hn.market.base.Method;
import com.hn.market.base.Module;
import com.hn.market.export.ChannelExport;

public class HNPhotoModule extends Module {
	private static final String TAG = HNPhotoModule.class.getSimpleName();
	
	private static final int TAKE_PICTURE = 0;
	private static final int CHOOSE_PICTURE = 1;
	private static final int CROP = 2;
	private static final int CROP_PICTURE = 3;
	
	private static HNPhotoModule _photoModule = null;
	
	private String _callback = "";
	private int _cropX = 500;
	private int _cropY = 500;
	public static HNPhotoModule getInstance() {
		if (_photoModule == null) {
			synchronized (HNPhotoModule.class) {
				if (_photoModule == null) {
					_photoModule = new HNPhotoModule();
				}
			}
		}
		return _photoModule;
	}

	public HNPhotoModule() {
		super("hnphoto");
		Register("getphoto",new GetPhoto());
		Log.d(TAG, "init hnphoto");
	}
	
	//截取图片
	private void cropImage(Uri uri, int outputX, int outputY, int requestCode) {
			Intent intent = new Intent("com.android.camera.action.CROP");  
	        intent.setDataAndType(uri, "image/*");  
	        intent.putExtra("crop", "true");  
	        intent.putExtra("aspectX", outputX);  
	        intent.putExtra("aspectY", outputY);  
	        intent.putExtra("outputX", outputX);   
	        intent.putExtra("outputY", outputY); 
	        intent.putExtra("outputFormat", "JPEG");
	        intent.putExtra("noFaceDetection", true);
	        intent.putExtra("return-data", true);  
		    activity.startActivityForResult(intent, requestCode);
	}
	
	//Bitmap进行base64编码
	private String bitmapToBase64(Bitmap bitmap) {  
		  
	    String result = null;  
	    ByteArrayOutputStream baos = null;  
	    try {  
	        if (bitmap != null) {  
	            baos = new ByteArrayOutputStream();  
	            bitmap.compress(Bitmap.CompressFormat.JPEG, 100, baos);  
	  
	            baos.flush();  
	            baos.close();  
	  
	            byte[] bitmapBytes = baos.toByteArray();  
	            result = Base64.encodeToString(bitmapBytes, Base64.DEFAULT);  
	        }  
	    } catch (IOException e) {  
	        e.printStackTrace();  
	    } finally {  
	        try {  
	            if (baos != null) {  
	                baos.flush();  
	                baos.close();  
	            }  
	        } catch (IOException e) {  
	            e.printStackTrace();  
	        }  
	    }  
	    return result;  
	}  
	
	private Bitmap compressImage(Bitmap image)
	{
		ByteArrayOutputStream baos = new ByteArrayOutputStream();  
        image.compress(Bitmap.CompressFormat.JPEG, 100, baos);//质量压缩方法，这里100表示不压缩，把压缩后的数据存放到baos中
        int options = 100;
        while ( baos.toByteArray().length / 1024 > 100) {  //循环判断如果压缩后图片是否大于100kb,大于继续压缩        
       	    baos.reset();//重置baos即清空baos  
            image.compress(Bitmap.CompressFormat.JPEG, options, baos);//这里压缩options%，把压缩后的数据存放到baos中
            options -=10;//每次都减少10，注意有可能为负，为负会报错  
            if(options < 10) break;
        }  
        ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());//把压缩后的数据baos存放到ByteArrayInputStream中  
        Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, null);//把ByteArrayInputStream数据生成图片
        return bitmap;
	}
	
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (resultCode == Activity.RESULT_OK) {
			switch (requestCode) {
			case TAKE_PICTURE:
				/*
				//将保存在本地的图片取出并缩小后显示在界面上
				Bitmap bitmap = BitmapFactory.decodeFile(Environment.getExternalStorageDirectory()+"/image.jpg");
				Bitmap newBitmap = ImageTools.zoomBitmap(bitmap, bitmap.getWidth() / SCALE, bitmap.getHeight() / SCALE);
				//由于Bitmap内存占用较大，这里需要回收内存，否则会报out of memory异常
				bitmap.recycle();
				
				//将处理过的图片显示在界面上，并保存到本地
				iv_image.setImageBitmap(newBitmap);
				ImageTools.savePhotoToSDCard(newBitmap, Environment.getExternalStorageDirectory().getAbsolutePath(), String.valueOf(System.currentTimeMillis()));
				*/
				break;
			case CHOOSE_PICTURE:
				/*
				ContentResolver resolver = activity.getContentResolver();
				//照片的原始资源地址
				Uri originalUri = data.getData(); 
	            try {
	            	//使用ContentProvider通过URI获取原始图片
					Bitmap photo = MediaStore.Images.Media.getBitmap(resolver, originalUri);
					if (photo != null) {
						//为防止原始图片过大导致内存溢出，这里先缩小原图显示，然后释放原始Bitmap占用的内存
						Bitmap smallBitmap = ImageTools.zoomBitmap(photo, photo.getWidth() / 100, photo.getHeight() / 100);
						//释放原始图片占用的内存，防止out of memory异常发生
						photo.recycle();
						
						String base64 = bitmapToBase64(smallBitmap);
						if(_callback != "")
						{
							ChannelExport.getInstance().executeAyncMethod(_callback,base64);
						}
					}
				} catch (FileNotFoundException e) {
				    e.printStackTrace();
				} catch (IOException e) {
					e.printStackTrace();
				}
				break;
				*/
			case CROP:
			{
				Uri uri = null;
				if (data != null) {
					uri = data.getData();
				}else {
					String fileName = activity.getSharedPreferences("temp",Context.MODE_WORLD_WRITEABLE).getString("tempName", "");
					uri = Uri.fromFile(new File(Environment.getExternalStorageDirectory(),fileName));
				}
				cropImage(uri, _cropX, _cropY, CROP_PICTURE);
			}
				break;
			
			case CROP_PICTURE:
			{
				Bitmap photo = null;
				Uri photoUri = data.getData();
				if (photoUri != null) {
					photo = BitmapFactory.decodeFile(photoUri.getPath());
				}
				if (photo == null) {
					Bundle extra = data.getExtras();
					if (extra != null) {
		                photo = (Bitmap)extra.get("data");  
		            }  
				}
				Bitmap smallPhoto = null;
				if(photo != null) {//压缩
					smallPhoto = compressImage(photo);
				}
				String base64 = bitmapToBase64(smallPhoto);
				if(_callback != "") {
					ChannelExport.getInstance().executeAyncMethod(_callback,base64);
				}
			}
				break;
			default:
				break;
			}
		}
	}
	
	public class GetPhoto implements Method {

		@Override
		public String Execute(String args, String callBack) {
			JSONObject json;
			try {
				json = new JSONObject(args);
				_cropX = json.getInt("x");
				_cropY = json.getInt("y");
			} catch (JSONException e) {
				e.printStackTrace();
			}
			_callback = callBack;
			activity.runOnUiThread(new Runnable(){
				@Override
				public void run() {
					
					AlertDialog.Builder builder = new AlertDialog.Builder(activity);
					builder.setTitle("请选择图片来源");
					builder.setNegativeButton("取消", null);
					builder.setItems(new String[]{"拍照","相册"}, new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {
							switch (which) {
							case TAKE_PICTURE:
							{
								Uri imageUri = null;
								String fileName = null;
								Intent openCameraIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
								//删除上一次截图的临时文件
								SharedPreferences sharedPreferences = activity.getSharedPreferences("temp",Context.MODE_WORLD_WRITEABLE);
								ImageTools.deletePhotoAtPathAndName(Environment.getExternalStorageDirectory().getAbsolutePath(), sharedPreferences.getString("tempName", ""));
								//保存本次截图临时文件名字
								fileName = String.valueOf(System.currentTimeMillis()) + ".jpg";
								Editor editor = sharedPreferences.edit();
								editor.putString("tempName", fileName);
								editor.commit();
								imageUri = Uri.fromFile(new File(Environment.getExternalStorageDirectory(),fileName));
								//指定照片保存路径（SD卡），image.jpg为一个临时文件，每次拍照后这个图片都会被替换
								openCameraIntent.putExtra(MediaStore.EXTRA_OUTPUT, imageUri);
								activity.startActivityForResult(openCameraIntent, CROP);
							}
							break;
								
							case CHOOSE_PICTURE:
							{
								Intent openAlbumIntent = new Intent(Intent.ACTION_GET_CONTENT);
								openAlbumIntent.setDataAndType(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, "image/*");
								activity.startActivityForResult(openAlbumIntent, CROP);
							}
							break;

							default:
								break;
							}
						}
					});
					builder.create().show();
				}
				});
			return "";
		}
	}
}

