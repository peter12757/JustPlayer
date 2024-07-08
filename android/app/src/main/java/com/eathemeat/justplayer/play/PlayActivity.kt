package com.eathemeat.justplayer.play

import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import android.view.View
import android.widget.AutoCompleteTextView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.constraintlayout.compose.Visibility
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.viewmodel.CreationExtras
import androidx.recyclerview.widget.DefaultItemAnimator
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.eathemeat.justplayer.databinding.ActivityPlayBinding
import com.eathemeat.justplayer.databinding.IncludePlayListBinding
import com.google.android.material.slider.Slider

class PlayActivity : AppCompatActivity() {

    lateinit var binding:ActivityPlayBinding
    lateinit var model:PlayViewModel
    val TAG = "PlayActivity"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        model = ViewModelProvider(this).get(PlayViewModel::class.java)
        binding = ActivityPlayBinding.inflate(layoutInflater)
        setContentView(binding.root)
        ViewCompat.setOnApplyWindowInsetsListener(binding.root) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        //view
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
//                    setAdapter()
                }
                rvPlaylist.apply {
                    layoutManager = LinearLayoutManager(context).apply { orientation = RecyclerView.VERTICAL }
                    adapter = PlayListAdapter()
                    addItemDecoration(DividerItemDecoration(context,RecyclerView.VERTICAL))
                    setItemAnimator(DefaultItemAnimator())
                }
                ibPlaylistControl.setOnClickListener(){ view ->
                    view.isSelected = !view.isSelected
                    rvPlaylist.visibility.let {
                        if(view.isSelected) Visibility.Visible else Visibility.Gone
                    }
                }
            }

            //surface
            surface.holder.apply {
                addCallback(object: SurfaceHolder.Callback2{
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
                }else {
                    clPlaycontrol.visibility = View.VISIBLE
                }
            }
            btnPlay.setOnClickListener() {
                Log.d(TAG, "btnPlay:setOnClickListener ")
                model.prepare()
                model.start()
            }
            btnNext.setOnClickListener() {
                Log.d(TAG, "btnNext: setOnClickListener")
            }
            btnPre.setOnClickListener() {
                Log.d(TAG, "btnPre: setOnClickListener")
            }
            proPro.addOnChangeListener() { slider: Slider, fl: Float, b: Boolean ->
                Log.d(TAG, "proPro: addOnChangeListener ${fl} - ${b} ")
                model.seekTo(fl.toLong())
            }

        }

    }
}