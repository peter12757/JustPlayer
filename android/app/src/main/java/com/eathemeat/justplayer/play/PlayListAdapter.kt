package com.eathemeat.justplayer.play

import android.content.Intent
import android.util.Log
import android.util.Range
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.content.res.AppCompatResources
import androidx.recyclerview.widget.RecyclerView
import androidx.recyclerview.widget.RecyclerView.ViewHolder
import com.eathemeat.justplayer.R
import com.eathemeat.justplayer.data.PlayItem
import com.eathemeat.justplayer.databinding.ItemPlayfileBinding
import com.eathemeat.justplayer.databinding.ItemPlaylistBinding
import com.eathemeat.justplayer.databinding.ItemPlaynetBinding

class PlayListAdapter() : RecyclerView.Adapter<PlayListAdapter.PlayListViewHolder>() {

    var data:MutableList<PlayItem> = mutableListOf()
    val TAG = "PlayListAdapter"

    val TYPE_PLAYITEM = 0X01
    val TYPE_SOURCE_FILE = TYPE_PLAYITEM+1
    val TYPE_SOURCE_NET = TYPE_SOURCE_FILE+1

    inner class PlayListViewHolder(var itemView: View) : ViewHolder(itemView){
        fun onBindViewHolder(position: Int, playItem: PlayItem) {
            when(itemViewType) {
                TYPE_PLAYITEM->{
                    ItemPlaylistBinding.bind(itemView).apply {
                        txtName.text = playItem.name
                        txtDesc.text = playItem.desc
                        txtDesc.visibility = View.GONE
                        ibPlay.setOnClickListener() {

                        }
                        ibMore.setOnClickListener() {
                            if (it.isSelected) {
                                txtDesc.visibility = View.VISIBLE
                                ibMore.apply { setImageDrawable(AppCompatResources.getDrawable(context,R.drawable.keyboard_arrow_down_48px))}
                            } else {
                                txtDesc.visibility = View.GONE
                                ibMore.apply { setImageDrawable(AppCompatResources.getDrawable(context,R.drawable.keyboard_arrow_up_48px))}
                            }
                        }
                    }
                }
                TYPE_SOURCE_FILE->{
                    ItemPlayfileBinding.bind(itemView).apply {
                        clPlayFile.setOnClickListener(){
                            var intent = Intent(Intent.ACTION_PICK)
                            intent.type = "video/*"
                            itemView.context.startActivity(intent)
                        }
                    }
                }
                TYPE_SOURCE_NET->{
                    ItemPlaynetBinding.bind(itemView).apply {
                        clPlayNet.setOnClickListener() {
                            Log.d(TAG, "onBindViewHolder() called")
                            //todo net dialog add play item
                        }
                    }
                }
            }
        }

    }

    override fun getItemViewType(position: Int): Int {
        return when(position) {
            itemCount ->{
                TYPE_SOURCE_FILE
            }
            itemCount+1 ->{
                TYPE_SOURCE_NET
            }
            else -> TYPE_PLAYITEM
        }
    }



    override fun onCreateViewHolder(
        parent: ViewGroup,
        viewType: Int
    ): PlayListViewHolder {
        var inflater = LayoutInflater.from(parent.context)
        var view = ItemPlaylistBinding.inflate(inflater).root
        when(viewType){
            TYPE_PLAYITEM ->{
            }
            TYPE_SOURCE_FILE->{
                view = ItemPlayfileBinding.inflate(inflater).root
            }
            TYPE_SOURCE_NET ->{
                view = ItemPlaynetBinding.inflate(inflater).root
            }
            else ->{
                Log.e(
                    TAG,
                    "onCreateViewHolder() called with: parent = $parent, viewType = $viewType"
                )
            }

        }
        return PlayListViewHolder(view)
    }

    override fun getItemCount(): Int {
        return data.size+2
    }

    override fun onBindViewHolder(
        holder: PlayListViewHolder,
        position: Int
    ) {
        holder.onBindViewHolder(position,data[position])
    }

}