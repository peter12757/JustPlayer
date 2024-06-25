package com.eathemeat.justplayer.play

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.content.res.AppCompatResources
import androidx.constraintlayout.compose.Visibility
import androidx.recyclerview.widget.RecyclerView
import androidx.recyclerview.widget.RecyclerView.ViewHolder
import com.eathemeat.justplayer.R
import com.eathemeat.justplayer.data.PlayItem
import com.eathemeat.justplayer.databinding.ItemPlaylistBinding

class PlayListAdapter() : RecyclerView.Adapter<PlayListAdapter.PlayListViewHolder>() {

    var data:MutableList<PlayItem> = mutableListOf()




    class PlayListViewHolder(var binding: ItemPlaylistBinding) : ViewHolder(binding.root){
        fun onBindViewHolder(position: Int, playItem: PlayItem) {
            binding.txtName.text = playItem.name
            binding.txtDesc.text = playItem.desc
            binding.txtDesc.visibility = View.GONE
            binding.ibPlay.setOnClickListener() {

            }
            binding.ibMore.setOnClickListener() {
                if (it.isSelected) {
                    binding.txtDesc.visibility = View.VISIBLE
                    binding.ibMore.apply { setImageDrawable(AppCompatResources.getDrawable(context,R.drawable.keyboard_arrow_down_48px))}
                } else {
                    binding.txtDesc.visibility = View.GONE
                    binding.ibMore.apply { setImageDrawable(AppCompatResources.getDrawable(context,R.drawable.keyboard_arrow_up_48px))}
                }
            }
        }

    }

    override fun onCreateViewHolder(
        parent: ViewGroup,
        viewType: Int
    ): PlayListViewHolder {
        var binding = ItemPlaylistBinding.inflate(LayoutInflater.from(parent.context))
        return PlayListViewHolder(binding)
    }

    override fun getItemCount(): Int {
        return data.size
    }

    override fun onBindViewHolder(
        holder: PlayListViewHolder,
        position: Int
    ) {
        holder.onBindViewHolder(position,data[position])
    }

}