/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package ca.welcomelm.crossRoad;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.annotation.TargetApi;
import android.app.WallpaperManager;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.Point;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;
import android.widget.Toast;

import ca.welcomelm.crossRoad.iapUtil.IabHelper;
import ca.welcomelm.crossRoad.iapUtil.IabHelper.OnIabPurchaseFinishedListener;
import ca.welcomelm.crossRoad.iapUtil.IabHelper.OnIabSetupFinishedListener;
import ca.welcomelm.crossRoad.iapUtil.IabHelper.QueryInventoryFinishedListener;
import ca.welcomelm.crossRoad.iapUtil.IabResult;
import ca.welcomelm.crossRoad.iapUtil.Inventory;
import ca.welcomelm.crossRoad.iapUtil.Purchase;

import com.google.android.gms.ads.*;
import com.startapp.android.publish.Ad;
import com.startapp.android.publish.AdDisplayListener;
import com.startapp.android.publish.AdEventListener;
import com.startapp.android.publish.StartAppAd;
import com.startapp.android.publish.StartAppSDK;

public class crossRoad extends Cocos2dxActivity implements OnIabSetupFinishedListener, 
QueryInventoryFinishedListener, OnIabPurchaseFinishedListener{
	
	private static crossRoad _appActiviy;
	private IabHelper iabHelper;
	private static final String SKU_PRODUCT = "ca.welcomelm.continue.ads";
	private static final int requestBuy = 8;
	private boolean isIabSetup = false;
	private StartAppAd startAppAd = new StartAppAd(this);
	static String devId , appId;
	private static AdEventListener adEventListener;
	private static AdDisplayListener adDisplayListener;
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
//		LinearLayout.LayoutParams adParams = new LinearLayout.LayoutParams(
//				LinearLayout.LayoutParams.FILL_PARENT,
//		LinearLayout.LayoutParams.FILL_PARENT);

//		adView = new AdView(this);
		
//		LinearLayout main = (LinearLayout) LinearLayout.inflate(this, R.layout.adview, null);
//		
//		adView = (AdView) main.findViewById(R.id.adView);
		
//		adView.setAdSize(AdSize.BANNER);
//		adView.setAdUnitId(AD_UNIT_ID);

//		AdRequest adRequest = new AdRequest.Builder()
//		.addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
//		.build();
//
//		adView.loadAd(adRequest);
//		adView.setAdListener(new AdListener(){
//			public void onAdLeftApplication(){
//				onAdClicked();
//			}
//		});
	    
	    adDisplayListener = new AdDisplayListener() {
			@Override
			public void adClicked(Ad arg0) {
				// TODO Auto-generated method stub
				System.out.println("adClicked");
				onAdClicked();
			}
			@Override
			public void adDisplayed(Ad arg0) {
				// TODO Auto-generated method stub
				System.out.println("adDisplayed");
			}
			@Override
			public void adHidden(Ad arg0) {
				// TODO Auto-generated method stub
				System.out.println("adHidden");
				startAppAd.loadAd(adEventListener);
			}
	    };
	    
	    adEventListener = new AdEventListener(){

			@Override
			public void onFailedToReceiveAd(Ad arg0) {
				// TODO Auto-generated method stub
				System.out.println("onFailedToReceiveAd");
			}

			@Override
			public void onReceiveAd(Ad arg0) {
				// TODO Auto-generated method stub
				System.out.println("onReceiveAd");
			}
	    	
	    };
		
//        addContentView(main,adParams);

		_appActiviy = this;
		
		String base64EncodedPublicKey = 
                "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAtKmDndSMvICd6tadPMkZBUE4qezWOm9BS24x9luwE5+BgwKhXwnmf37xw0hGMZuyXQVkadqVb5PFUdMFd+F1EiyoEBAOSDNZlNyI/xnNmEjDKBObNInWtrb60KqCWqEPHM7mBHYMBr+c9b8xrjyXsRwzwPkrh3NGcBctB8KSKi99rJ+7pU9msis/boX2sglAZ3aV7+VLGrdhgI5663PZa+PElVKoVNuo/d3Pw6jB6RGutjrHcSKZNeeW8ruV1SFCjP5LNiCkRqe73fYVw21/B2vob7ePvcpyH96K8lZcgZXdtAqidoOYWa5QuV3bdXLdyrwwbcvmz++YIMeVPmOlOQIDAQAB";
		iabHelper = new IabHelper(this, base64EncodedPublicKey);
	}
    
    public native void onAdClicked();

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// crossRoad should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }
    
    public static void showAds()
    {
    	_appActiviy.runOnUiThread(new Runnable(){

    		@Override
    		public void run()
    		{
    			if(_appActiviy.startAppAd.isReady())
    				_appActiviy.startAppAd.showAd(adDisplayListener);
    			else
    				_appActiviy.startAppAd.loadAd(adEventListener);
    		}
    	});

    }
    
    public static void changeAdsId(String dev_id , String app_id)
    {
    	if(dev_id.equals(devId) && app_id.equals(appId)) return;
    	devId = dev_id;
    	appId = app_id;
    	System.out.println(devId);
    	System.out.println(appId);
    	_appActiviy.runOnUiThread(new Runnable(){

    		@Override
    		public void run()
    		{
    			StartAppSDK.init(_appActiviy, devId, appId, false);
    			_appActiviy.startAppAd.loadAd(adEventListener);
    		}
    	});

    }
	
	public static void purchase(){
		_appActiviy.runOnUiThread(new Runnable(){

    		@Override
    		public void run(){
    			if(_appActiviy.isIabSetup == true){
    				_appActiviy.iabHelper.queryInventoryAsync(_appActiviy);
    			}else{
    				_appActiviy.iabHelper.startSetup(_appActiviy);
    			}
    		}
    	});		
	}
	
	public native void onPaymentError();
	public native void onPaymentSuccess();

	@Override
	public void onIabSetupFinished(IabResult result) {
		// TODO Auto-generated method stub
		if (result.isFailure()){
			onPaymentError();
		}else{
			isIabSetup = true;
			iabHelper.queryInventoryAsync(this);
		}
	}

	@Override
	public void onQueryInventoryFinished(IabResult result, Inventory inv) {
		// TODO Auto-generated method stub
		if (result.isFailure()) {
			onPaymentError();
			return;
		}
		
		if (inv.hasPurchase(SKU_PRODUCT)){
			onPaymentSuccess();
		}else{
			iabHelper.launchPurchaseFlow(this, 
					SKU_PRODUCT, requestBuy, this);
		}
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		switch (requestCode) {
		case requestBuy:

		    // Pass on the activity result to the helper for handling
		    if (!iabHelper.handleActivityResult(requestCode, resultCode, data)) {
		        // not handled, so handle it ourselves (here's where you'd
		        // perform any handling of activity results not related to in-app
		        // billing...
		        super.onActivityResult(requestCode, resultCode, data);
		    }
		    break;

		default:
			super.onActivityResult(requestCode, resultCode, data);
			break;
		}	
	}

	@Override
	public void onIabPurchaseFinished(IabResult result, Purchase info) {
		// TODO Auto-generated method stub
		if (result.isFailure()) {
			onPaymentError();
			return;
		}
		
		if (info.getSku().equals(SKU_PRODUCT)) {
			iabHelper.queryInventoryAsync(this);
		}		
	}
	
	public native void up();
	public native void down();
	public native void end();
	
	public boolean onKeyDown (int keyCode, KeyEvent event) {
		if(keyCode == KeyEvent.KEYCODE_DPAD_UP){
			up();
		}else if(keyCode == KeyEvent.KEYCODE_DPAD_DOWN){
			down();
		}
		return false;
	}
	
	public boolean onKeyUp (int keyCode, KeyEvent event) {
		if(keyCode == KeyEvent.KEYCODE_DPAD_UP || keyCode == KeyEvent.KEYCODE_DPAD_DOWN){
			end();
		}
		return false;
	}
	
	@Override
	public void onPause() {
	    super.onPause();
	    startAppAd.onPause();
	}
	
	@Override
	public void onResume() {
	    super.onResume();
	    startAppAd.onResume();
	}
}
