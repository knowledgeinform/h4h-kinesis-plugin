# Add project specific ProGuard rules here.
# You can control the set of applied configuration files using the
# proguardFiles setting in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}

# Uncomment this to preserve the line number information for
# debugging stack traces.
#-keepattributes SourceFile,LineNumberTable

# If you keep the line number information, uncomment this to
# hide the original source file name.
#-renamesourcefileattribute SourceFile
-keepattributes Exceptions, Signature, InnerClasses
-keepclasseswithmembernames,includedescriptorclasses class * {
    native <methods>;
}

-keep class com.tomahawkrobotics.kinesis.utilities.* { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.statereport.* { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.types.* { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.controldata.* { *; }

### Begin Rules for System Setup ###

## Don't obfuscate drawables in the utilities/res/drawable folder ##
-keepdirectories src/main/res/**
-keeppackagenames com.tomahawkrobotics.kinesis.utilities.res.raw
-keeppackagenames com.tomahawkrobotics.kinesis.utilities.res.drawable
-keepattributes InnerClasses
-keep class **.R
-keep class **.R$* { <fields>; }

-keep class kotlin.Metadata
-keep class com.tomahawkrobotics.kinesis.utilities.jetpack_compose.* { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.jetpack_compose.** { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.jetpack_compose.composables.** { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.jetpack_compose.composables.setup_layout.** { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.jetpack_compose.composables.setup_layout.SetupLayoutKt { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.jetpack_compose.composables.DividedLayoutBodyKt { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.jetpack_compose.composables.ColoredTextButtonKt { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.jetpack_compose.composables.SubmitButtonWithErrorMessageKt { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.jetpack_compose.composables.LoadingIndicatorKt { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.system_setup.** { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.system_setup.models.* { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.system_setup.models.node_setup_activity_launch_intent.* { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.system_setup.models.radio.* { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.system_setup.models.radio.** { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.jetpack_compose.theme.** { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.jetpack_compose.theme.* { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.jetpack_compose.theme.ThemeKt.** { *; }
-keep class com.tomahawkrobotics.kinesis.utilities.jetpack_compose.theme.ThemeKt {
    public static <methods> ;
}
-keepclasseswithmembernames,includedescriptorclasses class com.tomahawkrobotics.kinesis.utilities.jetpack_compose.theme.* {
    static <methods>;
}
### End Rules for System Setup ###

-keep class com.tomahawkrobotics.kinesis.sdk.platform.** { *; }
-keep class com.tomahawkrobotics.kinesis.sdk.platform.KinesisVectorKt { *; }
-keep class com.tomahawkrobotics.kinesis.sdk.platform.KinesisQuaternion { *; }
-keepclassmembers class com.tomahawkrobotics.kinesis.sdk.platform.KinesisQuaternion {
    public static ** Companion;
}
-keep class com.tomahawkrobotics.kinesis.sdk.platform.KinesisVector { *; }
-keep class com.tomahawkrobotics.kinesis.sdk.platform.NodeFactory  { *; }
-keepclassmembers class com.tomahawkrobotics.kinesis.sdk.platform.NodeFactory {
    public static ** Companion;
}
-keep class com.tomahawkrobotics.kinesis.sdk.platform.KinesisNotification { *; }
-keepclassmembers class com.tomahawkrobotics.kinesis.sdk.platform.KinesisNotification {
    public static ** Companion;
}
-keep class com.tomahawkrobotics.kinesis.sdk.PluginStorageDelegate { *; }
-keep class com.tomahawkrobotics.kinesis.sdk.PluginStorageException.* { *; }
-keep class com.tomahawkrobotics.kinesis.sdk.PluginService { *; }
-keep class com.tomahawkrobotics.kinesis.sdk.MissionModuleInterface { *; }
-keep class com.tomahawkrobotics.kinesis.sdk.PoiData { *; }
-keep class com.tomahawkrobotics.kinesis.sdk.KinesisSdkCallbackInterface { *; }
-keep class com.tomahawkrobotics.kinesis.sdk.ImageProcessorInterface { *; }
-keep class com.tomahawkrobotics.kinesis.sdk.KinesisMissionModuleAccessPoint { *; }

## Rules for Gson
-keepattributes *Annotation*
-keep class com.google.gson.** { *; }

### Added to rectify 'Missing classes detected while running R8' Error
-dontwarn com.tomahawkrobotics.kinesis.utilities.jetpack_compose.theme.ThemeKt*

### Keeps https://proandroiddev.com/android-default-proguard-rules-guide-20058ba7a486
#
## Preserve the @Keep annotation
#-keep class androidx.annotation.Keep
#
### Do not remove or rename @Keep annotated elements like
## methods and classes which are accessed only via reflection
#-keep @androidx.annotation.Keep class * { *; }
#
## Preserve class and fields annotated with @Keep
## <init> matches any type of constructor in a class
#-keepclasseswithmembers class * {
#    @androidx.annotation.Keep <fields>;
#    @androidx.annotation.Keep <init>(...);
#}
## Preserve enum values() and valueOf() methods for all enums in the project
#-keepclassmembers enum * {
#    public static **[] values();
#    public static ** valueOf(java.lang.String);
#}
#
## Preserves CREATOR field in each vlass which implements
## Parcelable to be accessible in case of reflection
#-keepclassmembers class * implements android.os.Parcelable {
#    public static final ** CREATOR;
#}
