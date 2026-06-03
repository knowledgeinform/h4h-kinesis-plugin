package com.tomahawkrobotics.kinesis.plugins.salesdemo

import android.app.Application

/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

/** class MApplication
 * base application class required by android to support activities and services of any sort.
 */
class MApplication : Application() {
    @Synchronized
    override fun onCreate() {
        super.onCreate()
        context = this@MApplication
    }

    companion object {
        var context: MApplication? = null
    }
}
