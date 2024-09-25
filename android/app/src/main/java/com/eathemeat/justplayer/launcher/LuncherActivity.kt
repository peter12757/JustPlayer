@file:OptIn(ExperimentalMaterial3Api::class)

package com.eathemeat.justplayer.launcher

import android.Manifest
import android.app.AlertDialog
import android.content.DialogInterface
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.provider.Settings
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.core.app.ActivityCompat
import com.eathemeat.justplayer.launcher.screen.MainContainScreen
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme

class LauncherActivity : ComponentActivity() {
    
    private var wait_time = mutableIntStateOf(5)

    val REQUEST_EXTERNAL_STORAGE =1
    val PERMISSIONS_STORAGE = arrayOf(
        Manifest.permission.READ_EXTERNAL_STORAGE,
        Manifest.permission.WRITE_EXTERNAL_STORAGE
    )
    var havePermission = false
    lateinit var dialog:AlertDialog

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            JustPlayerTheme {
                // A surface container using the 'background' color from the them e
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    MainContainScreen ()
                }
            }
        }
    }

    override fun onResume() {
        super.onResume()
        checkPermission()

    }

    private fun checkPermission() {
        //检查权限（NEED_PERMISSION）是否被授权 PackageManager.PERMISSION_GRANTED表示同意授权
        if (Build.VERSION.SDK_INT >= 30) {
            if (!Environment.isExternalStorageManager()) {
                dialog = AlertDialog.Builder(this)
                    .setTitle("提示")//设置标题
                    .setMessage("请开启文件访问权限，否则无法正常使用本应用！")
                    .setNegativeButton("取消",  { dialogInterface: DialogInterface, i: Int ->
                            dialog.dismiss()
                    })
                    .setPositiveButton("确定", DialogInterface.OnClickListener() { dialogInterface: DialogInterface, i: Int ->
                            dialog.dismiss();
                            intent = Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION);
                            startActivity(intent);
                    }).create()
                dialog.show()
            } else {
                havePermission = true;
                Log.i("swyLog", "Android 11以上，当前已有权限");
            }
        } else {
            if (Build.VERSION.SDK_INT > Build.VERSION_CODES.M) {
                if (ActivityCompat.checkSelfPermission(
                        this,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE
                    ) != PackageManager.PERMISSION_GRANTED
                ) {
                    //申请权限
                    dialog = AlertDialog.Builder (this)
                        .setTitle("提示")//设置标题
                        .setMessage("请开启文件访问权限，否则无法正常使用本应用！")
                        .setPositiveButton("确定", DialogInterface . OnClickListener () { dialogInterface: DialogInterface, i: Int ->
                                dialog.dismiss();
                                ActivityCompat.requestPermissions(
                                    this,
                                    PERMISSIONS_STORAGE,
                                    REQUEST_EXTERNAL_STORAGE
                                )
                        }).create();
                    dialog.show();
                } else {
                    havePermission = true;
                    Log.i("swyLog", "Android 6.0以上，11以下，当前已有权限");
                }
            } else {
                havePermission = true;
                Log.i("swyLog", "Android 6.0以下，已获取权限");
            }
        }
    }
}

private val timeColorArr = arrayOf(Color.Red,Color.Black,Color.Blue, Color.Gray,Color.Yellow,Color.White)
const val TAG = "LauncherActivity"

