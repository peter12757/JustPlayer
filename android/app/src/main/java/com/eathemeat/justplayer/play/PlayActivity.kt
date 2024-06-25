package com.eathemeat.justplayer.play

import android.os.Bundle
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.constraintlayout.compose.Visibility
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.recyclerview.widget.DefaultItemAnimator
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.eathemeat.justplayer.databinding.ActivityPlayBinding

class PlayActivity : AppCompatActivity() {

    lateinit var binding:ActivityPlayBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        binding = ActivityPlayBinding.inflate(layoutInflater)
        setContentView(binding.root)
        ViewCompat.setOnApplyWindowInsetsListener(binding.root) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        //playlist
        binding.rvPlaylist.apply {
            layoutManager = LinearLayoutManager(context).apply { orientation = RecyclerView.VERTICAL }
            adapter = PlayListAdapter()
            addItemDecoration(DividerItemDecoration(context,RecyclerView.VERTICAL))
            setItemAnimator(DefaultItemAnimator())
        }
        binding.ibPlaylistControl.setOnClickListener(){ view ->
            view.isSelected = !view.isSelected
            binding.rvPlaylist.visibility.let {
                if(view.isSelected) Visibility.Visible else Visibility.Gone
            }
        }

        //playcontrol



    }



}