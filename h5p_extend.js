jQuery(window).load(function() {
	jQuery('.h5p-slides-wrapper').on('mouseover', '.h5p-draggable img', function (e) {
		var imgSrc = jQuery(this).attr('src');

		var imgSrcSplit = imgSrc.split('/');
		var last = imgSrcSplit[imgSrcSplit.length-1]
		var imgNameExcludeExt = last.split('.');
		var imgName = imgNameExcludeExt[0];

		//console.log(imgSrcSplit.length-1 +' =='+imgSrcSplit[imgSrcSplit.length-1]);
		//imgSrcSplit[imgSrcSplit.length-1] = imgName+'.mp3';
		var audioSrc = imgSrcSplit.join('/');
		audioSrc = imgSrc.replace('images', 'audios');
		audioSrc = audioSrc.replace('.png', '.mp3');
		audioSrc = audioSrc.replace('.jpg', '.mp3');
		audioSrc = audioSrc.replace('.gif', '.mp3');
		audioSrc = audioSrc.replace('.jpeg', '.mp3');
		audioSrc = audioSrc.replace('.JPG', '.mp3');
		//console.log('audioSrc=='+audioSrc);
		//console.log('imgName=='+imgName);
		//console.log('audio file len=='+jQuery('#audio_'+imgName).length);
		var audioFileLen = jQuery('#audio_'+imgName).length;
		console.log('audioFileLen=='+audioFileLen);
		if (audioFileLen == 0) {
		
		var imgWidth = jQuery(this).attr('width');
		var imgWidthAfter = imgWidth.replace('%', '');
		var imgWidthAfterParse = parseInt(imgWidthAfter);
		var imgWidthReduceSize = imgWidthAfterParse-27;

		//console.log('imgWidthReduceSize=='+imgWidthReduceSize);

		jQuery(this).css('width', imgWidthReduceSize+'%');

		jQuery(this).parent().prepend('<div id="control_'+imgName+'" class="speaker_controls">&nbsp;</div><audio id="audio_'+imgName+'"><source src="'+audioSrc+'" type="audio/mpeg"><source src="http://demo.codesamplez.com/audio/music.ogg" type="audio/ogg"></audio>');

		//binding play events on speaker btn.
		jQuery( "#control_"+imgName).bind( "click", function() {
			//jQuery("#audio_"+imgName).trigger('play');
			jQuery("#audio_"+imgName).trigger('play');
/*
			if (jQuery("#audio_"+imgName).paused == false) {
			      jQuery("#audio_"+imgName).trigger('pause');
			      console.log('music paused');
			} else {
			      jQuery("#audio_"+imgName).trigger('play');
			      console.log('music playing');
  			} */
		});	
		}

		//var thisChildLen = jQuery('.'+jQuery(this).attr('class') +' > i').length;
		//var thisChild = jQuery(this).find('.child[class="fa"]');
		//console.log('this child length ='+ jQuery(this).html());
		//if (thisChild.length == 0) {
		//	jQuery(this).append('<i class="fa fa-volume-off"></i>');
		//}
		//jQuery(this).append('<i class="fa fa-volume-off"></i>');

/*
   jQuery(this).hover(function () {
    jQuery("#audio_test").trigger('play');
},function(){
      jQuery("#audio_test").trigger('pause');  
});
*/
	});
/*
jQuery(".h5p-draggable").draggable({
	revert: 'invalid',
	snap: true,
	stop: function(event, ui) {
		alert('drag');
	}
});

jQuery(".h5p-dropzone").droppable({
tolerance: 'fit',
greedy: false,
refreshPositions: true,
drop: function(event, ui) {
	alert('drop');
  var placeholderId = this.id;
  var placeholderIdArray = placeholderId.split('_');
  var placeholderFid = placeholderIdArray[1];

  var imageId = ui.draggable.attr("id");
  var imageDraggedArray = imageId.split('_');
  var imageFid = imageDraggedArray[1];

  if(placeholderFid == imageFid) {
    answerCount++;
  }
  dragCount++;

  $("#" + imageId).draggable("option", "disabled", true);
  $('#' + placeholderId).droppable("option", "disabled", true);
  $('#dropCount').val(dragCount);
  $('#answerCount').val(answerCount);

}
});*/
});
