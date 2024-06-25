<?php

function green_taxonomy_manager_voc_homepage() {
  $vocabularies = taxonomy_get_vocabularies();
  if (isset($vocabularies) && !empty($vocabularies)) {
    foreach ($vocabularies as $key => $val) {
      $vocab_options[$key] = $val->name;
    }
  }
  $form['green_base_vocabulary'] = array(
      '#type' => 'select',
      '#title' => t('Select Vabulary'),
      '#options' => isset($vocab_options) ? $vocab_options : array(''),
      '#default_value' => variable_get('green_base_vocabulary'),
      '#description' => t('Select base vocabulary for your site homepage.'),
  );
  return system_settings_form($form);
}

function green_homepage() {
  if (!user_access('Green Landing Page')) {
  //drupal_access_denied();
  }
  $green_base_vocab = variable_get('green_base_vocabulary');
  $vocab_tree = taxonomy_get_tree($green_base_vocab, 0, 1);
  $homepage_data['subject'] = $vocab_tree;

  if (isset($vocab_tree) && !empty($vocab_tree)) {
    $grades = array();

    $subject = isset($_GET['subject']) ? filter_xss($_GET['subject']) : '';
    if (isset($subject) && $subject <> '') {
      $grade_vocab_tree = taxonomy_get_tree($green_base_vocab, $subject, 1);
      $grades[$subject] = $grade_vocab_tree;
    } else {
      foreach ($vocab_tree as $vocab_tree_key => $vocab_tree_val) {
        $grade_vocab_tree = taxonomy_get_tree($green_base_vocab, $vocab_tree_val->tid, 1);
        $grades[$vocab_tree_val->tid] = $grade_vocab_tree;
      }
    }
    $homepage_data['grade'] = $grades;
  }

  return theme('green_homepage', array('homepage_data' => $homepage_data));
}

function green_grades() {
  global $user;
  $green_base_vocab = variable_get('green_base_vocabulary');
  $green_grades_data = array();

  $grade = isset($_GET['grade']) ? filter_xss($_GET['grade']) : '';

  // Fetching grades subject..
  if (isset($grade) && $grade <> '') {
    $grade_data = taxonomy_term_load($grade);
    //dsm($grade_data);
    $green_grades_data['grade_data'] = $grade_data;

    $grade_data_vocab_tree = taxonomy_get_tree($green_base_vocab, $grade, 1);
    $green_grades_data['grade_subject'] = $grade_data_vocab_tree;
  }

  // Fetching grades topic..
  if (isset($green_grades_data['grade_subject']) && !empty($green_grades_data['grade_subject'])) {
    foreach ($green_grades_data['grade_subject'] as $grades_subject_data_key => $grades_subject_data_val) {
      $grade_topic_vocab_tree = taxonomy_get_tree($green_base_vocab, $grades_subject_data_val->tid, 1);
      $green_grades_data['topic'][$grades_subject_data_val->tid] = $grade_topic_vocab_tree;
    }
  }

  // Fetching grades sub topic..
  if (isset($green_grades_data['topic']) && !empty($green_grades_data['topic'])) {
	$interactive_data = $quiz_data = array();
    foreach ($green_grades_data['topic'] as $grades_topic_data_key => $grades_topic_data_val) {
      if (isset($grades_topic_data_val) && !empty($grades_topic_data_val)) {
        foreach ($grades_topic_data_val as $grades_topic_data_val_key => $grades_topic_data_val_val) {


		  ////////////Interactive, quiz content///////////////////

		$interactive_data_temp = array();
		$result_set = _green_fetch_content($grades_topic_data_val_val->tid, 'h5p_content');
		while ($row = $result_set->fetchObject()) {
      dsm($row);
		  $interactive_data_temp[$row->nid]['nid'] = $row->nid;
		  $interactive_data_temp[$row->nid]['title'] = $row->title;
      $is_new = _green_fetch_new_status($row->created);
      $interactive_data_temp[$row->nid]['is_new'] = $is_new;

		  if (isset($row->field_featuringpix_fid)) {
			$file = file_load($row->field_featuringpix_fid);
			$interactive_data_temp[$row->nid]['image_uri'] = image_style_url('thumbnail', $file->uri);
		  }

		  $user_vote_data_param['nid'] = $row->nid;
		  $user_vote_details = _green_user_vote_data_fetch($user_vote_data_param);
		  $vote_data = _green_votes_data_fetch($row->nid);
		  if (isset($vote_data) && !empty($vote_data)) {
			$values = array('user' => isset($user_vote_details->value) ? $user_vote_details->value : NULL, 'average' => $vote_data->average, 'count' => $vote_data->vote_count);
		  }
		  $entity = entity_load('node', array($row->nid));
		  $settings = array('entity' => $entity[key($entity)], /*'entity_id' => $row->nid,  'entity_type' => 'node', */'microdata' => array(), 'field_name' => 'field_rate',
			  'langcode' => LANGUAGE_NONE, 'labels_enable' => FALSE, 'title' => NULL, 'style' => 'smart', 'text' => 'average',
			  'content_type' => 'node', 'description' => NULL, 'content_id' => $row->nid, 'stars' => 5, 'autosubmit' => TRUE, 'allow_revote' => TRUE,
			  'allow_ownvote' => TRUE, 'allow_clear' => FALSE, 'tag' => 'vote');
		  $interactive_data_temp[$row->nid]['fivestar'] = render(drupal_get_form('fivestar_custom_widget', $values, $settings));

		  $interactive_data[$grades_topic_data_val_val->tid] = $interactive_data_temp;
      
		}


//		$green_grades_data['interactive_data'] = $interactive_data;

		$quiz_data_temp = array();
		$quiz_result_set = _green_fetch_content($grades_topic_data_val_val->tid, 'quiz');
		while ($row = $quiz_result_set->fetchObject()) {
		  $quiz_data_temp[$row->nid]['nid'] = $row->nid;
		  $quiz_data_temp[$row->nid]['title'] = $row->title;
      $is_new = _green_fetch_new_status($row->created);
      $quiz_data_temp[$row->nid]['is_new'] = $is_new;
		  if (isset($row->field_featuringpix_fid)) {
			$file = file_load($row->field_featuringpix_fid);
			$quiz_data_temp[$row->nid]['image_uri'] = image_style_url('thumbnail', $file->uri);
			;
		  }

		  $user_vote_data_param['nid'] = $row->nid;
		  $user_vote_details = _green_user_vote_data_fetch($user_vote_data_param);
		  $vote_data = _green_votes_data_fetch($row->nid);
		  if (isset($vote_data) && !empty($vote_data)) {
			$values = array('user' => isset($user_vote_details->value) ? $user_vote_details->value : NULL, 'average' => $vote_data->average, 'count' => $vote_data->vote_count);
		  }
		  $settings = array('entity_id' => $row->nid, 'microdata' => array(), 'entity_type' => 'node', 'field_name' => 'field_rate',
			  'langcode' => LANGUAGE_NONE, 'labels_enable' => FALSE, 'title' => NULL, 'style' => 'smart', 'text' => 'average',
			  'content_type' => 'node', 'description' => NULL, 'content_id' => $row->nid, 'stars' => 5, 'autosubmit' => TRUE, 'allow_revote' => TRUE,
			  'allow_ownvote' => TRUE, 'allow_clear' => FALSE, 'tag' => 'vote');
		  $quiz_data_temp[$row->nid]['fivestar'] = render(drupal_get_form('fivestar_custom_widget', $values, $settings));
		  $quiz_data[$grades_topic_data_val_val->tid] = $quiz_data_temp;
		}
//		$green_grades_data['quiz_data'] = $quiz_data;
		/////////////Interactive, quiz content/////////////////

		  if ((!isset($green_grades_data['quiz_data'][$grades_topic_data_val_val->tid]) || count($green_grades_data['quiz_data'][$grades_topic_data_val_val->tid]) == 0) &&
			  (!isset($green_grades_data['interactive_data'][$grades_topic_data_val_val->tid]) || count($green_grades_data['interactive_data'][$grades_topic_data_val_val->tid]) == 0)) {
			$grade_sub_topic_vocab_tree = taxonomy_get_tree($green_base_vocab, $grades_topic_data_val_val->tid, 1);
			$green_grades_data['sub_topic'][$grades_topic_data_val_val->tid] = $grade_sub_topic_vocab_tree;
		  }
        }
      }
    }
  }

  $user_vote_data_param = array();
  if ($user->uid == 0) {
    $user_vote_data_param['host'] = $user->hostname;
    $user_vote_data_param['uid'] = $user->uid;
  } else {
    $user_vote_data_param['uid'] = $user->uid;
  }

  // Fetching grades interactive data..
  if (isset($green_grades_data['sub_topic']) && !empty($green_grades_data['sub_topic'])) {

    foreach ($green_grades_data['sub_topic'] as $grades_sub_topic_data_key => $grades_sub_topic_data_val) {
      if (isset($grades_sub_topic_data_val) && !empty($grades_sub_topic_data_val)) {
        foreach ($grades_sub_topic_data_val as $grades_sub_topic_data_val_key => $grades_sub_topic_data_val_val) {
          $interactive_data_temp = array();
          $result_set = _green_fetch_content($grades_sub_topic_data_val_val->tid, 'h5p_content');
          while ($row = $result_set->fetchObject()) {
            $interactive_data_temp[$row->nid]['nid'] = $row->nid;
            $interactive_data_temp[$row->nid]['title'] = $row->title;
            $is_new = _green_fetch_new_status($row->created);
            $interactive_data_temp[$row->nid]['is_new'] = $is_new;

            if (isset($row->field_featuringpix_fid)) {
              $file = file_load($row->field_featuringpix_fid);
              $interactive_data_temp[$row->nid]['image_uri'] = image_style_url('thumbnail', $file->uri);
            }

            $user_vote_data_param['nid'] = $row->nid;
            $user_vote_details = _green_user_vote_data_fetch($user_vote_data_param);
            $vote_data = _green_votes_data_fetch($row->nid);
            if (isset($vote_data) && !empty($vote_data)) {
              $values = array('user' => isset($user_vote_details->value) ? $user_vote_details->value : NULL, 'average' => $vote_data->average, 'count' => $vote_data->vote_count);
            }
            $entity = entity_load('node', array($row->nid));
            $settings = array('entity' => $entity[key($entity)], /*'entity_id' => $row->nid,  'entity_type' => 'node', */'microdata' => array(), 'field_name' => 'field_rate',
                'langcode' => LANGUAGE_NONE, 'labels_enable' => FALSE, 'title' => NULL, 'style' => 'smart', 'text' => 'average',
                'content_type' => 'node', 'description' => NULL, 'content_id' => $row->nid, 'stars' => 5, 'autosubmit' => TRUE, 'allow_revote' => TRUE,
                'allow_ownvote' => TRUE, 'allow_clear' => FALSE, 'tag' => 'vote');
            $interactive_data_temp[$row->nid]['fivestar'] = render(drupal_get_form('fivestar_custom_widget', $values, $settings));

            $interactive_data[$grades_sub_topic_data_val_val->tid] = $interactive_data_temp;
          }

          $quiz_data_temp = array();
          $quiz_result_set = _green_fetch_content($grades_sub_topic_data_val_val->tid, 'quiz');
          while ($row = $quiz_result_set->fetchObject()) {
            $quiz_data_temp[$row->nid]['nid'] = $row->nid;
            $quiz_data_temp[$row->nid]['title'] = $row->title;
            $is_new = _green_fetch_new_status($row->created);
            $quiz_data_temp[$row->nid]['is_new'] = $is_new;
            if (isset($row->field_featuringpix_fid)) {
              $file = file_load($row->field_featuringpix_fid);
              $quiz_data_temp[$row->nid]['image_uri'] = image_style_url('thumbnail', $file->uri);
              ;
            }

            $user_vote_data_param['nid'] = $row->nid;
            $user_vote_details = _green_user_vote_data_fetch($user_vote_data_param);
            $vote_data = _green_votes_data_fetch($row->nid);
            if (isset($vote_data) && !empty($vote_data)) {
              $values = array('user' => isset($user_vote_details->value) ? $user_vote_details->value : NULL, 'average' => $vote_data->average, 'count' => $vote_data->vote_count);
            }
            $settings = array('entity_id' => $row->nid, 'microdata' => array(), 'entity_type' => 'node', 'field_name' => 'field_rate',
                'langcode' => LANGUAGE_NONE, 'labels_enable' => FALSE, 'title' => NULL, 'style' => 'smart', 'text' => 'average',
                'content_type' => 'node', 'description' => NULL, 'content_id' => $row->nid, 'stars' => 5, 'autosubmit' => TRUE, 'allow_revote' => TRUE,
                'allow_ownvote' => TRUE, 'allow_clear' => FALSE, 'tag' => 'vote');
            $quiz_data_temp[$row->nid]['fivestar'] = render(drupal_get_form('fivestar_custom_widget', $values, $settings));
            $quiz_data[$grades_sub_topic_data_val_val->tid] = $quiz_data_temp;
          }
        }
      }
    }
  }
  
  $green_grades_data['interactive_data'] = $interactive_data;
  $green_grades_data['quiz_data'] = $quiz_data;
  return theme('green_grades', array('green_grades_data' => $green_grades_data));
}

function _green_user_vote_data_fetch($user_vote_data_param) {
  $query = db_select('votingapi_vote', 'vv');
  $query->fields('vv', array('value'));
  $query->condition('vv.entity_type', 'node', '=');
  $query->condition('vv.entity_id', $user_vote_data_param['nid']);

  if (isset($user_vote_data_param['host']))
    $query->condition('vv.vote_source', $user_vote_data_param['host']);

  if (isset($user_vote_data_param['uid']))
    $query->condition('vv.uid', $user_vote_data_param['uid']);

  $result = $query->execute()->fetchObject();
  return $result;
}

function _green_votes_data_fetch($nid) {
  $query = db_select('votingapi_vote', 'vv');
  $query->addExpression('AVG(vv.value)', 'average');
  $query->addExpression('COUNT(*)', 'vote_count');
  $query->condition('vv.entity_type', 'node', '=');
  $query->condition('vv.entity_id', $nid);

  $result = $query->execute()->fetchObject();
  return $result;
}

function _green_fetch_content($tid, $bundle) {
  $query = db_select('node', 'n');
  $query->fields('n', array('nid', 'title', 'created'));
  $query->fields('fdfqi', array('field_featuringpix_fid'));
  $query->condition('fdfos.bundle', $bundle, '=');
  $query->condition('fdfos.entity_type', 'node', '=');
  $query->InnerJoin('field_data_field_yearly_plan', 'fdfos', 'fdfos.entity_id = n.nid');
  $query->leftJoin('field_data_field_featuringpix', 'fdfqi', 'fdfqi.entity_id = n.nid');
  $query->condition('n.status', NODE_PUBLISHED);
  $query->condition('fdfos.field_yearly_plan_tid', $tid);

  $result = $query->execute();
  return $result;
}

function green_subjects() {
  global $user;
  $green_grades_data = array();
  /////////////////////////////////////////////////////////////
  $green_base_vocab = variable_get('green_base_vocabulary');

  $grade = isset($_GET['grade']) ? filter_xss($_GET['grade']) : '';
  $subject = isset($_GET['subject']) ? filter_xss($_GET['subject']) : '';
  // Fetching grades subject..
  if (isset($grade) && $grade <> '') {
    $grade_data = taxonomy_term_load($grade);
    $green_grades_data['grade_data'] = $grade_data;

//	$grade_data_vocab_tree = taxonomy_get_tree($green_base_vocab, $grade, 1);
//	$green_grades_data['grade_subject'] = $grade_data_vocab_tree;
    $green_grades_data['grade_subject'][] = taxonomy_term_load($subject);
  }

  // Fetching grades topic..
  if (isset($green_grades_data['grade_subject']) && !empty($green_grades_data['grade_subject'])) {
    foreach ($green_grades_data['grade_subject'] as $grades_subject_data_key => $grades_subject_data_val) {
      $grade_topic_vocab_tree = taxonomy_get_tree($green_base_vocab, $grades_subject_data_val->tid, 1);
      $green_grades_data['topic'][$grades_subject_data_val->tid] = $grade_topic_vocab_tree;
    }
  }

  // Fetching grades sub topic..
  if (isset($green_grades_data['topic']) && !empty($green_grades_data['topic'])) {
	$interactive_data = $quiz_data = array();
    foreach ($green_grades_data['topic'] as $grades_topic_data_key => $grades_topic_data_val) {
      if (isset($grades_topic_data_val) && !empty($grades_topic_data_val)) {
        foreach ($grades_topic_data_val as $grades_topic_data_val_key => $grades_topic_data_val_val) {

		  ////////////Interactive, quiz content///////////////////

		$interactive_data_temp = array();
		$result_set = _green_fetch_content($grades_topic_data_val_val->tid, 'h5p_content');
		while ($row = $result_set->fetchObject()) {
		  $interactive_data_temp[$row->nid]['nid'] = $row->nid;
		  $interactive_data_temp[$row->nid]['title'] = $row->title;
      $is_new = _green_fetch_new_status($row->created);
      $interactive_data_temp[$row->nid]['is_new'] = $is_new;
		  if (isset($row->field_featuringpix_fid)) {
			$file = file_load($row->field_featuringpix_fid);
			$interactive_data_temp[$row->nid]['image_uri'] = image_style_url('thumbnail', $file->uri);
		  }

		  $user_vote_data_param['nid'] = $row->nid;
		  $user_vote_details = _green_user_vote_data_fetch($user_vote_data_param);
		  $vote_data = _green_votes_data_fetch($row->nid);
		  if (isset($vote_data) && !empty($vote_data)) {
			$values = array('user' => isset($user_vote_details->value) ? $user_vote_details->value : NULL, 'average' => $vote_data->average, 'count' => $vote_data->vote_count);
		  }
		  $entity = entity_load('node', array($row->nid));
		  $settings = array('entity' => $entity[key($entity)], /*'entity_id' => $row->nid,  'entity_type' => 'node', */'microdata' => array(), 'field_name' => 'field_rate',
			  'langcode' => LANGUAGE_NONE, 'labels_enable' => FALSE, 'title' => NULL, 'style' => 'smart', 'text' => 'average',
			  'content_type' => 'node', 'description' => NULL, 'content_id' => $row->nid, 'stars' => 5, 'autosubmit' => TRUE, 'allow_revote' => TRUE,
			  'allow_ownvote' => TRUE, 'allow_clear' => FALSE, 'tag' => 'vote');
		  $interactive_data_temp[$row->nid]['fivestar'] = render(drupal_get_form('fivestar_custom_widget', $values, $settings));

		  $interactive_data[$grades_topic_data_val_val->tid] = $interactive_data_temp;
		}


//		$green_grades_data['interactive_data'] = $interactive_data;

		$quiz_data_temp = array();
		$quiz_result_set = _green_fetch_content($grades_topic_data_val_val->tid, 'quiz');
		while ($row = $quiz_result_set->fetchObject()) {
		  $quiz_data_temp[$row->nid]['nid'] = $row->nid;
		  $quiz_data_temp[$row->nid]['title'] = $row->title;
      $is_new = _green_fetch_new_status($row->created);
      $quiz_data_temp[$row->nid]['is_new'] = $is_new;
		  if (isset($row->field_featuringpix_fid)) {
			$file = file_load($row->field_featuringpix_fid);
			$quiz_data_temp[$row->nid]['image_uri'] = image_style_url('thumbnail', $file->uri);
			;
		  }

		  $user_vote_data_param['nid'] = $row->nid;
		  $user_vote_details = _green_user_vote_data_fetch($user_vote_data_param);
		  $vote_data = _green_votes_data_fetch($row->nid);
		  if (isset($vote_data) && !empty($vote_data)) {
			$values = array('user' => isset($user_vote_details->value) ? $user_vote_details->value : NULL, 'average' => $vote_data->average, 'count' => $vote_data->vote_count);
		  }
		  $settings = array('entity_id' => $row->nid, 'microdata' => array(), 'entity_type' => 'node', 'field_name' => 'field_rate',
			  'langcode' => LANGUAGE_NONE, 'labels_enable' => FALSE, 'title' => NULL, 'style' => 'smart', 'text' => 'average',
			  'content_type' => 'node', 'description' => NULL, 'content_id' => $row->nid, 'stars' => 5, 'autosubmit' => TRUE, 'allow_revote' => TRUE,
			  'allow_ownvote' => TRUE, 'allow_clear' => FALSE, 'tag' => 'vote');
		  $quiz_data_temp[$row->nid]['fivestar'] = render(drupal_get_form('fivestar_custom_widget', $values, $settings));
		  $quiz_data[$grades_topic_data_val_val->tid] = $quiz_data_temp;
		}
//		$green_grades_data['quiz_data'] = $quiz_data;
		/////////////Interactive, quiz content/////////////////

		  if ((!isset($green_grades_data['quiz_data'][$grades_topic_data_val_val->tid]) || count($green_grades_data['quiz_data'][$grades_topic_data_val_val->tid]) == 0) &&
			  (!isset($green_grades_data['interactive_data'][$grades_topic_data_val_val->tid]) || count($green_grades_data['interactive_data'][$grades_topic_data_val_val->tid]) == 0)) {
          $grade_sub_topic_vocab_tree = taxonomy_get_tree($green_base_vocab, $grades_topic_data_val_val->tid, 1);
          $green_grades_data['sub_topic'][$grades_topic_data_val_val->tid] = $grade_sub_topic_vocab_tree;
			  }
        }
      }
    }
  }
  $user_vote_data_param = array();
  if ($user->uid == 0) {
    $user_vote_data_param['host'] = $user->hostname;
    $user_vote_data_param['uid'] = $user->uid;
  } else {
    $user_vote_data_param['uid'] = $user->uid;
  }
  // Fetching grades interactive data..
  if (isset($green_grades_data['sub_topic']) && !empty($green_grades_data['sub_topic'])) {

    foreach ($green_grades_data['sub_topic'] as $grades_sub_topic_data_key => $grades_sub_topic_data_val) {
      if (isset($grades_sub_topic_data_val) && !empty($grades_sub_topic_data_val)) {
        foreach ($grades_sub_topic_data_val as $grades_sub_topic_data_val_key => $grades_sub_topic_data_val_val) {
          $interactive_data_temp = array();
          $result_set = _green_fetch_content($grades_sub_topic_data_val_val->tid, 'h5p_content');
          while ($row = $result_set->fetchObject()) {
            $interactive_data_temp[$row->nid]['nid'] = $row->nid;
            $interactive_data_temp[$row->nid]['title'] = $row->title;
            $is_new = _green_fetch_new_status($row->created);
            $interactive_data_temp[$row->nid]['is_new'] = $is_new;
            if (isset($row->field_featuringpix_fid)) {
              $file = file_load($row->field_featuringpix_fid);
              $interactive_data_temp[$row->nid]['image_uri'] = image_style_url('thumbnail', $file->uri);
            }
            $user_vote_data_param['nid'] = $row->nid;
            $user_vote_details = _green_user_vote_data_fetch($user_vote_data_param);
            $vote_data = _green_votes_data_fetch($row->nid);
            if (isset($vote_data) && !empty($vote_data)) {
              $values = array('user' => isset($user_vote_details->value) ? $user_vote_details->value : NULL, 'average' => $vote_data->average, 'count' => $vote_data->vote_count);
            }
            $settings = array('entity_id' => $row->nid, 'microdata' => array(), 'entity_type' => 'node', 'field_name' => 'field_rate',
                'langcode' => LANGUAGE_NONE, 'labels_enable' => FALSE, 'title' => NULL, 'style' => 'smart', 'text' => 'average',
                'content_type' => 'node', 'description' => NULL, 'content_id' => $row->nid, 'stars' => 5, 'autosubmit' => TRUE, 'allow_revote' => TRUE,
                'allow_ownvote' => TRUE, 'allow_clear' => FALSE, 'tag' => 'vote');
            $interactive_data_temp[$row->nid]['fivestar'] = render(drupal_get_form('fivestar_custom_widget', $values, $settings));
            $interactive_data[$grades_sub_topic_data_val_val->tid] = $interactive_data_temp;
          }

          $quiz_data_temp = array();
          $quiz_result_set = _green_fetch_content($grades_sub_topic_data_val_val->tid, 'quiz');
          while ($row = $quiz_result_set->fetchObject()) {
            $quiz_data_temp[$row->nid]['nid'] = $row->nid;
            $quiz_data_temp[$row->nid]['title'] = $row->title;
            $is_new = _green_fetch_new_status($row->created);
            $quiz_data_temp[$row->nid]['is_new'] = $is_new;
            if (isset($row->field_featuringpix_fid)) {
              $file = file_load($row->field_featuringpix_fid);
              $quiz_data_temp[$row->nid]['image_uri'] = image_style_url('thumbnail', $file->uri);
              ;
            }
            $user_vote_data_param['nid'] = $row->nid;
            $user_vote_details = _green_user_vote_data_fetch($user_vote_data_param);
            $vote_data = _green_votes_data_fetch($row->nid);
            if (isset($vote_data) && !empty($vote_data)) {
              $values = array('user' => isset($user_vote_details->value) ? $user_vote_details->value : NULL, 'average' => $vote_data->average, 'count' => $vote_data->vote_count);
            }
            $settings = array('entity_id' => $row->nid, 'microdata' => array(), 'entity_type' => 'node', 'field_name' => 'field_rate',
                'langcode' => LANGUAGE_NONE, 'labels_enable' => FALSE, 'title' => NULL, 'style' => 'smart', 'text' => 'average',
                'content_type' => 'node', 'description' => NULL, 'content_id' => $row->nid, 'stars' => 5, 'autosubmit' => TRUE, 'allow_revote' => TRUE,
                'allow_ownvote' => TRUE, 'allow_clear' => FALSE, 'tag' => 'vote');
            $quiz_data_temp[$row->nid]['fivestar'] = render(drupal_get_form('fivestar_custom_widget', $values, $settings));
            $quiz_data[$grades_sub_topic_data_val_val->tid] = $quiz_data_temp;
          }
        }
      }
    }
  }

  $green_grades_data['interactive_data'] = $interactive_data;
  $green_grades_data['quiz_data'] = $quiz_data;
  ////////////////////////////////////////////////////////////

//print '<pre>'; print_r($green_grades_data); exit;
  return theme('green_subjects', array('green_grades_data' => $green_grades_data));
}

function _green_fetch_new_status($created) {
  $cur_time = time();
  $diff = $cur_time - $created;
  $diff_in_weeks = $diff/604800; // in weeks
  if ($diff_in_weeks <= 1) { // 1 week condition
      return TRUE;
  }
  else {
    return FALSE;
  }
}