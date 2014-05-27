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

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.annotation.TargetApi;
import android.graphics.Color;
import android.graphics.Point;
import android.os.Build;
import android.os.Bundle;
import android.view.Display;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;

import com.google.android.gms.ads.*;

public class crossRoad extends Cocos2dxActivity{
	
	private static crossRoad _appActiviy;
	private AdView adView;
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);


		LinearLayout.LayoutParams adParams = new LinearLayout.LayoutParams(
				LinearLayout.LayoutParams.FILL_PARENT,
		LinearLayout.LayoutParams.FILL_PARENT);

//		adView = new AdView(this);
		
		LinearLayout main = (LinearLayout) LinearLayout.inflate(this, R.layout.adview, null);
		
		adView = (AdView) main.findViewById(R.id.adView);
		
//		adView.setAdSize(AdSize.BANNER);
//		adView.setAdUnitId(AD_UNIT_ID);

		AdRequest adRequest = new AdRequest.Builder()
		.addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
		.build();

		adView.loadAd(adRequest);
        addContentView(main,adParams);

		_appActiviy = this;
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// crossRoad should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }
    
    public static void hideAd()
    {
    	_appActiviy.runOnUiThread(new Runnable(){

    		@Override
    		public void run(){
				if (_appActiviy.adView.isEnabled())
				_appActiviy.adView.setEnabled(false);
				if (_appActiviy.adView.getVisibility() != 4 )
				_appActiviy.adView.setVisibility(View.INVISIBLE);
    		}
    	});

    }


    public static void showAd()
    {
    	_appActiviy.runOnUiThread(new Runnable(){

    		@Override
    		public void run()
    		{  
				if (!_appActiviy.adView.isEnabled())
				_appActiviy.adView.setEnabled(true);
				if (_appActiviy.adView.getVisibility() == 4 )
				_appActiviy.adView.setVisibility(View.VISIBLE); 
    		}
    	});

    }

	@Override
	protected void onResume() {
		super.onResume();
		if (adView != null) {
			adView.resume();
		}
	}

	@Override
	protected void onPause() {
		if (adView != null) {
			adView.pause();
		}
		super.onPause();
	}

	@Override
	protected void onDestroy() {
		adView.destroy();
		super.onDestroy();
	}
}
