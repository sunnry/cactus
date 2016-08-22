package org.freedesktop.gstreamer.tutorial_3;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.RelativeLayout;

/**
 * Created by tony on 16-8-20.
 */
public class ControlLayout extends RelativeLayout {

    private int mRadius; //view group radius
    private int mBaseDrawRadius; //control base view draw radius
    private int mPadDrawRadius; //control pad view draw radius

    private float mLastX;
    private float mLastY;

    private float mCurX;
    private float mCurY;


    public ControlLayout(Context context, AttributeSet attrs){
        super(context,attrs);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec,int heightMeasureSpec){

        //according input parameters to get measure mode and measure value
        int width = MeasureSpec.getSize(widthMeasureSpec);
        int widthMode = MeasureSpec.getMode(widthMeasureSpec);

        int height = MeasureSpec.getSize(heightMeasureSpec);
        int heightMode = MeasureSpec.getMode(heightMeasureSpec);

        width = height = Math.min(width,height);

        if (width != height){
            Log.w("ControlLayout","ControlLayout width and height need to be equal");
        }

        setMeasuredDimension(width,height);

        mRadius = Math.min(getMeasuredHeight(),getMeasuredWidth())/2;
        mBaseDrawRadius = (mRadius*3)/4 ; //set mBaseDrawRadius 3/4 percent of mRadius
        mPadDrawRadius = mRadius/4; //set mPadDrawRadius 1/4 of mRadius

        final int count = getChildCount(); //get child view count, infact this should be 2: base and pad

        int childMode = MeasureSpec.EXACTLY;
        for(int i =0;i < count;i++){
            final View child = getChildAt(i);

            int makeMeasureSpec = -1;
            if(child.getId() == R.id.id_controlPad_base_view){
                makeMeasureSpec = MeasureSpec.makeMeasureSpec(mRadius*2,childMode); //control base view layout match parent
            }else if (child.getId() == R.id.id_controlPad_pad_view){
                makeMeasureSpec = MeasureSpec.makeMeasureSpec((int)(mRadius/4)*2,childMode);
            }
            child.measure(makeMeasureSpec,makeMeasureSpec);
        }

    }

    public interface OnControlPadClickListener
    {
        void controlPadClick(View view,int pos);
    }

    private OnControlPadClickListener mControlPadClickListener;

    public void setControlPadClickListener(OnControlPadClickListener listener){
        this.mControlPadClickListener = listener;
    }

    @Override
    protected void onLayout(boolean changed, int l,int t,int r,int b){
        int layoutRadius = mRadius;
        final int childCount = getChildCount();

        for(int i = 0;i<childCount; i++){
            final View child = getChildAt(i);
            if (child.getId() == R.id.id_controlPad_base_view){
                child.layout(0,0,mRadius*2,mRadius*2);
            }else if(child.getId() == R.id.id_controlPad_pad_view){
                int left = (int)(mRadius -mPadDrawRadius + mCurX);
                int top = (int)(mRadius - mPadDrawRadius + mCurY);
                child.layout(left,top,left+mPadDrawRadius*2,top+mPadDrawRadius*2);
            }
        }


    }


    @Override
    public boolean dispatchTouchEvent(MotionEvent event){
        float x = event.getX();
        float y = event.getY();

        switch (event.getAction()){
            case MotionEvent.ACTION_DOWN:
                mLastX = x;
                mLastY = y;
                break;

            case MotionEvent.ACTION_MOVE:
                getPosition(x,y);
                requestLayout();
                break;

            case MotionEvent.ACTION_UP:
                mCurX = 0;  //when release, return back to center of parent
                mCurY = 0;
                requestLayout();
                break;
        }
        return super.dispatchTouchEvent(event);
    }

    @Override public boolean onTouchEvent(MotionEvent event){
        return true;
    }

    private void getPosition(float xTouch,float yTouch){

        double x = xTouch -mRadius;
        double y = yTouch - mRadius;
        double l = Math.hypot(x,y);

        if(l > mBaseDrawRadius){
            mCurX = (float) (x*(mBaseDrawRadius / l));
            mCurY = (float) (y*(mBaseDrawRadius / l));
        }else{
            mCurX = (float)x;
            mCurY = (float)y;
        }


    }

    /*
    private float getAngle(float xTouch,float yTouch){
        double x = xTouch - mRadius;
        double y = yTouch - mRadius;
        return (float) (Math.asin(y / Math.hypot(x, y)) * 180 / Math.PI);
    }

    private int getQuadrant(float x, float y){

        int tmpX = (int)(x - mRadius);
        int tmpY = (int)(y - mRadius);

        if(tmpX >= 0) {
            return tmpY >= 0 ? 4 : 1;
        }else{
            return tmpY >= 0 ? 3 : 2;
        }

    }
    */

}
