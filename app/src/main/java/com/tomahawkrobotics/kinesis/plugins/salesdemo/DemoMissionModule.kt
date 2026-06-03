package com.tomahawkrobotics.kinesis.plugins.salesdemo

import android.content.Context
import android.util.Log
import com.tomahawkrobotics.kinesis.sdk.MissionModuleInterface
import com.tomahawkrobotics.kinesis.sdk.PoiData
import java.io.BufferedReader
import java.io.IOException
import java.io.InputStreamReader

/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

/**
 * @class DemoMissionModule
 * sample object for how to use and implement MissionModuleInterface objects
 */
class DemoMissionModule(
    /**
     * Application context for the mission module creator/owner
     */
    mApplicationContext: Context?
) : MissionModuleInterface {
    /**
     * Current enable state of the mission module
     */
    private var mEnableState = false

    /**
     * YAML string data, this is loaded from file
     */
    private var mYaml = ""


    init {
        if (mApplicationContext != null) {
            val yamlStream =
                mApplicationContext.resources
                    .openRawResource(R.raw.demo_mission_module_ui)
            try {
                BufferedReader(InputStreamReader(yamlStream)).use { reader ->
                    var line = reader.readLine()
                    while (line != null) {
                        mYaml += line + "\n"
                        line = reader.readLine()
                    }
                }
            } catch (e: IOException) {
                mYaml = "none"
                Log.w(TAG, "Mission Module Raw file not found")
            }
        }
    }

    override fun getModuleLayoutYaml(): String {
        return mYaml
    }

    override fun getName(): String {
        return "DemoMissionModule"
    }

    override fun enable() {
        Log.d(TAG, "Enabling Demo Mission Module")
        mEnableState = true
    }

    override fun disable() {
        Log.d(TAG, "Disabling Demo Mission Module")
        mEnableState = false
    }

    override fun getEnableState(): Boolean {
        return mEnableState
    }

    override fun processButton(buttonId: String, buttonState: Boolean) {
        Log.d(TAG, "Button $buttonId received value $buttonState")
    }

    override fun processCheckbox(checkboxId: String, checkboxState: Boolean) {
        Log.d(TAG, "Checkbox $checkboxId received value $checkboxState")
    }

    override fun processTextField(textId: String, textData: String) {
        Log.d(TAG, "Text field $textId received value '$textData'")
    }

    override fun processPointOfInterestSelection(selectionId: String, data: PoiData) {
        Log.d(
            TAG,
            "POI Selection " +
                    selectionId +
                    " received value: " +
                    data.name +
                    ", " +
                    data.latitude +
                    ", " +
                    data.longitude
        )
    }

    companion object {
        /**
         * Log Tag
         */
        private val TAG: String = DemoMissionModule::class.java.simpleName
    }
}
