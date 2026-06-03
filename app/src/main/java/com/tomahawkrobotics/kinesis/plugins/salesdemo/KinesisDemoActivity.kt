package com.tomahawkrobotics.kinesis.plugins.salesdemo

import android.os.Bundle
import android.util.Log
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity

/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */


/**
 * @class KinesisDemoActivity
 * Provides example of how to create an activity for Android.  This particular activity is used
 * to start agent nodes for this plugin.
 */
class KinesisDemoActivity : AppCompatActivity() {
    /**
     * Start the hovering vehicle
     */
    private fun startHoveringVehicle() {
        mDemoService?.startHoveringVehicle()
    }

    /**
     * Start the ground vehicle
     */
    private fun startGroundVehicle() {
        mDemoService?.startGroundVehicle()
    }

    /**
     * Start the fixed wing vehicle
     */
    private fun startFixedWingVehicle() {
        mDemoService?.startFixedWingVehicle()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Log.v(TAG, "Lifecycle:onCreate called")

        // onCreate matches with onDestroy

        // Setup the View
        setContentView(R.layout.activity_main)

        /*
         * Start Hovering Vehicle button handle
         */
        val mStartHvButton = findViewById<Button>(R.id.start_hv_button)
        mStartHvButton.setOnClickListener { startHoveringVehicle() }
        /*
 * Start Ground Vehicle button handle
 */
        val mStartGvButton = findViewById<Button>(R.id.start_gv_button)
        mStartGvButton.setOnClickListener { startGroundVehicle() }
        /*
 * Start Fixed Wing Vehicle button handle
 */
        val mStartFwButton = findViewById<Button>(R.id.start_fw_button)
        mStartFwButton.setOnClickListener { startFixedWingVehicle() }
    }

    override fun onStart() {
        super.onStart()
        Log.v(TAG, "Lifecycle:onStart called")
        // onStart matches with onStop
    }

    override fun onResume() {
        super.onResume()
        Log.v(TAG, "Lifecycle:onResume called")
        // onPause matches with onResume
    }

    override fun onPause() {
        super.onPause()
        Log.v(TAG, "Lifecycle:onPause called")
        // onPause matches with onResume
    }

    override fun onStop() {
        super.onStop()
        Log.v(TAG, "Lifecycle:onStop called")
        // onStart matches with onStop
    }

    public override fun onDestroy() {
        super.onDestroy()
        Log.v(TAG, "Lifecycle:onDestroy called")
        // onCreate matches with onDestroy
    }

    companion object {
        private val TAG: String = KinesisDemoActivity::class.java.simpleName

        private var mDemoService: KinesisDemoService? = null

        /**
         * Set the associated service.  This also serves as a gate to prevent
         * starting vehicles before kinesis allows it.
         *
         * @param service target demo service
         */
        @JvmStatic
        fun setDemoService(service: KinesisDemoService?) {
            mDemoService = service
        }
    }
}
