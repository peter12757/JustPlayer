package com.eathemeat.justplayer.play

import android.Manifest
import android.content.pm.ActivityInfo
import android.content.pm.PackageManager
import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import android.view.View
import android.widget.AutoCompleteTextView
import android.widget.FrameLayout
import android.widget.LinearLayout
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.constraintlayout.compose.Visibility
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.lifecycle.ViewModelProvider
import androidx.recyclerview.widget.DefaultItemAnimator
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.eathemeat.justplayer.databinding.ActivityPlayBinding
import com.eathemeat.justplayer.databinding.IncludePlayListBinding

class PlayActivity : AppCompatActivity() {

    lateinit var binding:ActivityPlayBinding
    lateinit var model:PlayViewModel
    val TAG = "PlayActivity"
    val permission = arrayOf(Manifest.permission.READ_EXTERNAL_STORAGE,
        Manifest.permission.WRITE_EXTERNAL_STORAGE,
        Manifest.permission.MANAGE_EXTERNAL_STORAGE,
        Manifest.permission.INTERNET,
        Manifest.permission.ACCESS_NETWORK_STATE)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE
        model = ViewModelProvider(this).get(PlayViewModel::class.java)
        binding = ActivityPlayBinding.inflate(layoutInflater)
        setContentView(binding.root)
        ViewCompat.setOnApplyWindowInsetsListener(binding.root) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        //view
        initViews()

        initpermissions()

        observer()
    }

    private fun observer() {
        model.apply {
            videoSize.observe(this@PlayActivity) { size->
                var parentWidth = binding.llSurface.width
                var parentHeight = binding.llSurface.height
                Log.d(TAG, "observer() called with: size = $size parWidth:$parentWidth   parHeight:$parentHeight")
                var lp = LinearLayout.LayoutParams(binding.surface.layoutParams).apply {
                    if (size.first*parentHeight > parentWidth*size.second) {
                        width = parentWidth
                        height = size.first*size.second/parentWidth
                    }else {
                        width = size.first*size.second/parentHeight
                        height = parentHeight

                    }
                }
                binding.surface.layoutParams = lp
            }
        }
    }

    private fun initpermissions() {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE)
            != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this,
                permission,0)
        }
    }

    private fun initViews() {
        binding.apply {

            //playlist
            IncludePlayListBinding.bind(binding.incPlaylist.root).apply {
                mactPlaypath.apply {
                    threshold = 0
                    onFocusChangeListener = View.OnFocusChangeListener { v, hasFocus ->
                        if (v is AutoCompleteTextView) {
                            if (hasFocus) {
                                v.showDropDown()
                            } else {
                                v.dismissDropDown()
                            }
                        }
                    }
                    mactPlaypath.setText(model.fileGetter.mPath)
    //                    setAdapter()
                    model.scanfile()
                }
                rvPlaylist.apply {
                    layoutManager =
                        LinearLayoutManager(context).apply { orientation = RecyclerView.VERTICAL }
                    adapter = PlayListAdapter(model).apply {
                        model.mPlayList.observe(this@PlayActivity){ playList ->
                            data.clear()
                            data.addAll(playList)
                        }
                    }
                    addItemDecoration(DividerItemDecoration(context, RecyclerView.VERTICAL))
                    setItemAnimator(DefaultItemAnimator())
                }
                ibPlaylistControl.setOnClickListener() { view ->
                    view.isSelected = !view.isSelected
                    rvPlaylist.visibility.let {
                        if (view.isSelected) Visibility.Visible else Visibility.Gone
                    }
                }
            }

            //surface
            surface.holder.apply {
                addCallback(object : SurfaceHolder.Callback2 {
                    override fun surfaceCreated(holder: SurfaceHolder) {
                        Log.d(TAG, "surfaceCreated() called with: holder = $holder")
                        model.setSurface(holder.surface)
                    }

                    override fun surfaceChanged(
                        holder: SurfaceHolder,
                        format: Int,
                        width: Int,
                        height: Int
                    ) {
                        Log.d(
                            TAG,
                            "surfaceChanged() called with: holder = $holder, format = $format, width = $width, height = $height"
                        )
                    }

                    override fun surfaceDestroyed(holder: SurfaceHolder) {
                        Log.d(TAG, "surfaceDestroyed() called with: holder = $holder")
                        model.setSurface(null)
                    }

                    override fun surfaceRedrawNeeded(holder: SurfaceHolder) {
                        Log.d(TAG, "surfaceRedrawNeeded() called with: holder = $holder")
                    }
                })
            }
            //playcontrol
            surface.setOnClickListener() {
                if (clPlaycontrol.visibility == View.VISIBLE) {
                    clPlaycontrol.visibility = View.GONE
                } else {
                    clPlaycontrol.visibility = View.VISIBLE
                }
            }
            btnPlay.setOnClickListener() {
                Log.d(TAG, "btnPlay:setOnClickListener ")
                model.play(model.mCurPlayItem)
            }
            btnNext.setOnClickListener() {
                Log.d(TAG, "btnNext: setOnClickListener")
            }
            btnPre.setOnClickListener() {
                Log.d(TAG, "btnPre: setOnClickListener")
            }
            proPro.addOnValueChangeListener { slider, value, fromUser ->
                Log.d(
                    TAG,
                    "onCreate() called with: slider = $slider, value = $value, fromUser = $fromUser"
                )
                if (fromUser)
                    model.seekTo(value.toLong())
            }
        }
    }
}