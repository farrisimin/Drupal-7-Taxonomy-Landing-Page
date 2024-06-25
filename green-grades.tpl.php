<?php
global $base_path;
//print_r($green_grades_data);  exit;
$final = 0;
foreach ($green_grades_data['grade_subject'] as $grade_subject_key => $grade_subject_val) {
  $total_sub_sec_blocks = 0;
  if (isset($green_grades_data['topic'][$grade_subject_val->tid]) && !empty($green_grades_data['topic'][$grade_subject_val->tid])) {
    foreach ($green_grades_data['topic'][$grade_subject_val->tid] as $topic_key => $topic_val) {
      if (isset($green_grades_data['sub_topic'][$topic_val->tid]) && !empty($green_grades_data['sub_topic'][$topic_val->tid])) {
        $total_sub_sec_blocks = $total_sub_sec_blocks + count($green_grades_data['sub_topic'][$topic_val->tid]);
      }
      else {
        $total_sub_sec_blocks = $total_sub_sec_blocks + 1;
      }
    }
    if ($total_sub_sec_blocks == 0) {
      $subject_width = (121 * count($green_grades_data['topic'][$grade_subject_val->tid])) + count($green_grades_data['topic'][$grade_subject_val->tid]);
    } else {
      $subject_width = (121 * $total_sub_sec_blocks) + count($green_grades_data['topic'][$grade_subject_val->tid]);
    }
  } else {
    $subject_width = 470;
  }
  $final = $final + $subject_width;
}
?>
<div class="content clearFix">
  <!-- wrapper -->
  <div class="wrapper">
    <!-- things to do -->
    <div class="font_10 grade_section make_relative ">
      <div class="fontWhite section_head_area"><p><?php print mb_substr($green_grades_data['grade_data']->name, 0, 30, 'utf-8') ?></p></div>
    </div>
    <div class="clearFix reading-canvas">
      <div class="flL"  style="width:<?php print $final ?>px;">
<?php
if (isset($green_grades_data['grade_subject']) && !empty($green_grades_data['grade_subject'])) {

  foreach ($green_grades_data['grade_subject'] as $grade_subject_key => $grade_subject_val) {
    $total_sub_sec_blocks = 0;
    if (isset($green_grades_data['topic'][$grade_subject_val->tid]) && !empty($green_grades_data['topic'][$grade_subject_val->tid])) {
      foreach ($green_grades_data['topic'][$grade_subject_val->tid] as $topic_key => $topic_val) {
        if (isset($green_grades_data['sub_topic'][$topic_val->tid]) && !empty($green_grades_data['sub_topic'][$topic_val->tid])) {
          $total_sub_sec_blocks = $total_sub_sec_blocks + count($green_grades_data['sub_topic'][$topic_val->tid]);
        }
        else {
          $total_sub_sec_blocks = $total_sub_sec_blocks + 1;
        }
      }
      if ($total_sub_sec_blocks == 0) {
        $subject_width = (120 * count($green_grades_data['topic'][$grade_subject_val->tid])) + count($green_grades_data['topic'][$grade_subject_val->tid]);
      } else {
        $subject_width = (120 * $total_sub_sec_blocks) + count($green_grades_data['topic'][$grade_subject_val->tid]);
      }
    } else {
      $subject_width = 470;
    }
    ?>
            <div class="marginTB5 font_9 make_relative flL" style="width: <?php print $subject_width; ?>px; margin-right:1px;">
            <?php
            $params = array();
            $params['subject'] = $grade_subject_val->tid;
            $params['grade'] = $green_grades_data['grade_data']->tid;
//            $grade_subject_val->name = 'Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry\'s standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.';
            $grade_subject_title = (strlen($grade_subject_val->name) > 100) ? mb_substr($grade_subject_val->name, 0, 100, 'utf-8'). '..' : $grade_subject_val->name ;
            $link_text = '<div class="fontWhite subject_section section_head_area">
			<p>'. $grade_subject_title .'</p>
		  </div>';
            print l($link_text, 'green/subjects', array('html' => TRUE, 'query' => $params, 'attributes' => array(
                            'title' => $grade_subject_val->name
                            )));
            ?>

              <?php if (isset($green_grades_data['topic'][$grade_subject_val->tid]) && !empty($green_grades_data['topic'][$grade_subject_val->tid])) { ?>
                <div class="flL">
                <?php
                $all_equal = TRUE;
                foreach ($green_grades_data['topic'][$grade_subject_val->tid] as $topic_key => $topic_val) {
                  if (isset($green_grades_data['sub_topic'][$topic_val->tid]) && !empty($green_grades_data['sub_topic'][$topic_val->tid])) {
                    $all_equal = FALSE;
                    break;
                  }
                }
                foreach ($green_grades_data['topic'][$grade_subject_val->tid] as $topic_key => $topic_val) {
                  if (isset($green_grades_data['sub_topic'][$topic_val->tid]) && !empty($green_grades_data['sub_topic'][$topic_val->tid])) {
                    $ts_width = (119 * count($green_grades_data['sub_topic'][$topic_val->tid])) + count($green_grades_data['sub_topic'][$topic_val->tid]);
                  } else {
                    if ($all_equal === TRUE) {
                      $ts_width = ($subject_width / count($green_grades_data['topic'][$grade_subject_val->tid])) - 1;
                    }
                    else {
                      $ts_width =  120;
                    }
                  }
                  ?>
                    <div class="marginTB5 font_8 make_relative flL" style="width: <?php print $ts_width; ?>px; margin-right:1px;">

                    <?php
                    $params = array();
//					$params['subject'] = $topic_val->tid;
                    $topic_title = (strlen($topic_val->name) > 100) ? mb_substr($topic_val->name, 0, 100, 'utf-8'). '..' : $topic_val->name;
                    $link_text = '<div class="fontWhite topic_section section_head_area">
			<p>' . $topic_title .'</p>
		  </div>';
                    print l($link_text, 'taxonomy/term/' . $topic_val->tid, array('html' => TRUE, 'query' => $params, 'attributes' => array(
                                    'title' => $topic_val->name
                                    )));
                    ?>

                      <?php
                            if (isset($green_grades_data['interactive_data'][$topic_val->tid]) && !empty($green_grades_data['interactive_data'][$topic_val->tid])) {
                              ?>
                              <?php
                              foreach ($green_grades_data['interactive_data'][$topic_val->tid] as $interactive_key => $interactive_val) {
                                if (isset($interactive_val['image_uri']))
                                  $file_url = $interactive_val['image_uri'];
                                else
                                  $file_url = $base_path . drupal_get_path('module', 'green') . '/image/default_lessons.png';
                                
                                $params = array();
				$margin_left = (int)$ts_width-35;
//						  $params['subject'] = $interactive_val['nid'];
                                $link_text = '<div class="marginTB5 font_8 make_relative" style="width:"'.$ts_width.'"px;">';
if ($interactive_val['is_new']) {
$link_text .= '<div style="width:"'.$ts_width.'"px;" class="new-flag"><span style="width:"'.$ts_width.'"px; margin-left:"'.$margin_left.'"px;" class="new-txt">NEW</span></div>';
}
				$link_text .= '<div class="fontWhite standard_section section_head_area">
              <div class="cover_grade">
				  <div class="cover-image-container">
				  <div class="cover-outer-align">
				  <div class="cover-inner-align">
				  <img src="' . $file_url . '"/>
				  </div> 
				  </div> 
				  </div> 
				  </div>
          <div>
							  <p>' . mb_substr($interactive_val['title'], 0, 100, 'utf-8') . '..' . '</p></div>
                  <div class="fiveStarBlock">'.$interactive_val['fivestar'].'</div>
							</div>
              
						  </div>';
                                print l($link_text, 'node/' . $interactive_val['nid'], array('html' => TRUE, 'query' => $params, 'attributes' => array(
                                                'title' => $interactive_val['title']
                                                )));
                              }
                              ?>
                              <?php
                            }
							elseif (isset($green_grades_data['quiz_data'][$topic_val->tid]) && !empty($green_grades_data['quiz_data'][$topic_val->tid])) {
                              foreach ($green_grades_data['quiz_data'][$topic_val->tid] as $quiz_key => $quiz_val) {
                                if (isset($quiz_val['image_uri']))
                                  $file_url = $quiz_val['image_uri'];
                                else
                                  $file_url = $base_path . drupal_get_path('module', 'green') . '/image/default_quiz.png';
                                
                                $params = array();
				$margin_left = (int)$ts_width-35;
//						  $params['subject'] = $quiz_val['nid'];
                                $link_text = '<div class="marginTB5 font_8 make_relative" style="width:"'.$ts_width.'"px;">';
if ($quiz_val['is_new']) {
$link_text .= '<div style="width:"'.$ts_width.'"px;" class="new-flag"><span style="width:"'.$ts_width.'"px; margin-left: "'.$margin_left.'"px;" class="new-txt">NEW</span></div>';}

				$link_text .= '<div class="fontWhite standard_section_quiz section_head_area">
              <div class="cover_grade">
				  <div class="cover-image-container">
				  <div class="cover-outer-align">
				  <div class="cover-inner-align">
				  <img src="' . $file_url . '"/>
				  </div> 
				  </div> 
				  </div> 
				  </div>
          <div>
							  <p>' . mb_substr($quiz_val['title'], 0, 100, 'utf-8') . '..' . '</p></div>
                  <div class="fiveStarBlock">'.$quiz_val['fivestar'].'</div>
							</div>
						  </div>';
                                print l($link_text, 'node/' . $quiz_val['nid'], array('html' => TRUE, 'query' => $params, 'attributes' => array(
                                                'title' => $quiz_val['title']
                                                )));
                              }
                            }
					  elseif (isset($green_grades_data['sub_topic'][$topic_val->tid]) && !empty($green_grades_data['sub_topic'][$topic_val->tid])) {
                        foreach ($green_grades_data['sub_topic'][$topic_val->tid] as $sub_topic_key => $sub_topic_val) {
                          ?>
                          <div class="marginTB5 font_7 make_relative flL" style="width: 119px; margin-right:1px;">
                          <?php
                          $params = array();
//						  $params['subject'] = $sub_topic_val->tid;
                          $sub_topic_title = (strlen($sub_topic_val->name) > 80) ? mb_substr($sub_topic_val->name, 0, 80, 'utf-8'). '..' : $sub_topic_val->name;
                          $link_text = '<div class="fontWhite subtopic_section section_head_area">
			  <p>' . $sub_topic_title . '</p>
			</div>';
                          print l($link_text, 'taxonomy/term/' . $sub_topic_val->tid, array('html' => TRUE, 'query' => $params, 'attributes' => array(
                                          'title' => $sub_topic_val->name
                                          )));
                          ?>

                            <?php
                            if (isset($green_grades_data['interactive_data'][$sub_topic_val->tid]) && !empty($green_grades_data['interactive_data'][$sub_topic_val->tid])) {
                              ?>
                              <?php
                              foreach ($green_grades_data['interactive_data'][$sub_topic_val->tid] as $interactive_key => $interactive_val) {
                                if (isset($interactive_val['image_uri']))
                                  $file_url = $interactive_val['image_uri'];
                                else
                                  $file_url = $base_path . drupal_get_path('module', 'green') . '/image/default_lessons.png';
                                
                                $params = array();
//						  $params['subject'] = $interactive_val['nid'];
                                $link_text = '<div class="marginTB5 font_8 make_relative" style="width: 120px;">';
if ($interactive_val['is_new']) {
$link_text .= '<div style="width:120px;" class="new-flag"><span style="width:120px; margin-left:98px;" class="new-txt">NEW</span></div>';
}
				$link_text .= '<div class="fontWhite standard_section section_head_area">
              <div class="cover_grade">
				  <div class="cover-image-container">
				  <div class="cover-outer-align">
				  <div class="cover-inner-align">
				  <img src="' . $file_url . '"/>
				  </div> 
				  </div> 
				  </div> 
				  </div>
          <div>
							  <p>' . mb_substr($interactive_val['title'], 0, 100, 'utf-8') . '..' . '</p></div>
                  <div class="fiveStarBlock">'.$interactive_val['fivestar'].'</div>
							</div>
              
						  </div>';
                                print l($link_text, 'node/' . $interactive_val['nid'], array('html' => TRUE, 'query' => $params, 'attributes' => array(
                                                'title' => $interactive_val['title']
                                                )));
                              }
                              ?>
                              <?php
                            }

                            if (isset($green_grades_data['quiz_data'][$sub_topic_val->tid]) && !empty($green_grades_data['quiz_data'][$sub_topic_val->tid])) {
                              foreach ($green_grades_data['quiz_data'][$sub_topic_val->tid] as $quiz_key => $quiz_val) {
                                if (isset($quiz_val['image_uri']))
                                  $file_url = $quiz_val['image_uri'];
                                else
                                  $file_url = $base_path . drupal_get_path('module', 'green') . '/image/default_quiz.png';
                                
                                $params = array();
//						  $params['subject'] = $quiz_val['nid'];
                                $link_text = '<div class="marginTB5 font_8 make_relative"  style="width: 120px;">';
if ($quiz_val['is_new']) {
$link_text .= '<div style="width:120px;" class="new-flag"><span style="width:120px; margin-left:98px;" class="new-txt">NEW</span></div>';
}
				$link_text .= '<div class="fontWhite standard_section_quiz section_head_area">
              <div class="cover_grade">
				  <div class="cover-image-container">
				  <div class="cover-outer-align">
				  <div class="cover-inner-align">
				  <img src="' . $file_url . '"/>
				  </div> 
				  </div> 
				  </div> 
				  </div>
          <div>
							  <p>' . mb_substr($quiz_val['title'], 0, 100, 'utf-8') . '..' . '</p></div>
                  <div class="fiveStarBlock">'.$quiz_val['fivestar'].'</div>         
							</div>
						  </div>';
                                print l($link_text, 'node/' . $quiz_val['nid'], array('html' => TRUE, 'query' => $params, 'attributes' => array(
                                                'title' => $quiz_val['title']
                                                )));
                              }
                            }
                            ?>
                          </div>
                            <?php
                          }
                        }
                        ?>
                    </div>
                    <?php }
                    ?>
                </div>
                <?php }
                ?>
            </div>
              <?php
            }
          }
          ?>
      </div>
    </div>
  </div>
</div>
<script>
		(function($){
			$(window).load(function(){
				
				$(".reading-canvas").mCustomScrollbar({
					axis:"yx",
					theme:"3d",
					scrollInertia:550,
					scrollbarPosition:"outside"
				});
				
			});
		})(jQuery);
	</script>
