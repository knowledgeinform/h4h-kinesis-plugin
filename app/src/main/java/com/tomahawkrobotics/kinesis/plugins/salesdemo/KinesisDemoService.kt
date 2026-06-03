package com.tomahawkrobotics.kinesis.plugins.salesdemo

import android.app.Service
import android.content.Intent
import android.os.Binder
import android.os.IBinder
import android.os.Process
import android.util.Log
import com.tomahawkrobotics.kinesis.plugins.salesdemo.KinesisDemoActivity.Companion.setDemoService
import com.tomahawkrobotics.kinesis.plugins.salesdemo.KinesisH4HActivity.setService
import com.tomahawkrobotics.kinesis.sdk.KinesisMissionModuleAccessPoint

/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

class KinesisDemoService : Service() {
    /**
     * Mission Module object
     */
    private var demoMissionModule: DemoMissionModule? = null

    /**
     * Mission Module access point for loading the mission module to kinesis
     */
    private var missionModuleAccessPoint: KinesisMissionModuleAccessPoint = KinesisMissionModuleAccessPoint(
        INTEGRATOR_ID
    )

    /**
     * Local command to start the hovering vehicle
     */
    fun startHoveringVehicle() {
        Log.d(TAG, "Received command to start hovering vehicle")
        startNode(NODE_TYPE_DEMO_HOVERING, "hover", "", "", "", "")
    }

    /**
     * Local command to start the fixed wing vehicle
     */
    fun startFixedWingVehicle() {
        Log.d(TAG, "Received command to start fixed wing vehicle")
        startNode(NODE_TYPE_DEMO_FIXED_WING, "fw","", "", "", "")
    }

    /**
     * Local command to start the ground vehicle
     */
    fun startGroundVehicle() {
        Log.d(TAG, "Received command to start tracked vehicle")
        startNode(NODE_TYPE_DEMO_TRACKED, "gnd","", "", "", "")
    }

    fun startH4HVehicle(name: String, type: String, controlPort: String, statusPort: String, camURL: String) {
        Log.d(TAG, "Received command to start H4H vehicle")
        startNode(NODE_TYPE_H4H, name, type, controlPort, statusPort, camURL)
    }

    override fun onCreate() {
        Log.d(TAG, "onCreate() starting...")
        super.onCreate()

        // Start SDK
        if (!startSdk()) {
            Log.e(TAG, "Kinesis SDK failed to start")
            Process.killProcess(Process.myPid())
        }

        // Allow the activity to start a vehicle
//        setDemoService(this)
        setService(this)
        demoMissionModule = DemoMissionModule(applicationContext)
        missionModuleAccessPoint.registerMissionModule(demoMissionModule)

        Log.d(TAG, "onCreate() finished")
    }

    /**
     * Captures intents and operates on them
     *
     * @param intent  - broadcast intent
     * @param flags   - associated flags
     * @param startId - who sent the intent
     * @return
     */
    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        // Where all the work gets done
        if (intent != null && intent.action != null) {
            val action = intent.action
            Log.d(TAG, "onHandleIntent : $action")
        }
        return START_NOT_STICKY
    }

    override fun onDestroy() {
        super.onDestroy()
        Log.d(TAG, "onDestroy() called")
        stopSdk()
        Log.d(TAG, "killing process...")
        Process.killProcess(Process.myPid())
    }

    override fun onBind(intent: Intent): IBinder {
        val binder: IBinder = Binder()
        Log.i(TAG, "Binding Kinesis Test Service")
        return binder
    }

    override fun onUnbind(intent: Intent): Boolean {
        // According to https://developer.android.com/reference/android/app/Service.html#ProcessLifecycle
        // onUnbind is only called if no client with the Context.BIND_AUTO_CREATE flag exists.
        // Our IPC uses this flag, so this will never be called unless all kinesis applications have closed.
        Log.v(TAG, "onUnbind Called")
        return false
    }

    companion object {
        private val TAG: String = KinesisDemoService::class.java.simpleName

        //************************************************************************
        //              C++ Implemented Functions
        //************************************************************************
        init {
            System.loadLibrary("kinesis-salesdemo")
        }

        private const val NODE_TYPE_DEMO_HOVERING = 0
        private const val NODE_TYPE_DEMO_TRACKED = 1
        private const val NODE_TYPE_DEMO_FIXED_WING = 2
        private const val NODE_TYPE_H4H = 3


        /**
         * Integrator ID for kinesis
         */
        private const val INTEGRATOR_ID = "demo"
    }

    private external fun startSdk(): Boolean

    private external fun stopSdk()

    private external fun startNode(nodeType: Int, pName: String, pType: String, pcontrolPort: String, pstatusPort: String, pcamURL: String)
}
