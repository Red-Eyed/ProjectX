// В этом файле содержатся скрипты, которые используются на всех страницах профиля пользователя.

// Функция для голосования за карму.

function userKarmaVote(link, target_id, target_type, vote){
  $.post('/json/vote/',{ti:target_id, tt:target_type, v:vote},function(json){
    if(json.messages == 'ok'){
      var karma = $('.js-karma');

      // выделим отмеченный пункт.
      if(vote === 1){
        karma.addClass('voted_plus');
        $('.js-vote_plus',karma).attr({ title: 'Вы проголосовали положительно.', disabled: true});
        $('.js-vote_minus',karma).attr({title: 'Понизить карму.', disabled: false});
      }else if(vote === -1){
        karma.addClass('voted_minus');
        $('.js-vote_minus',karma).attr({ title: 'Вы проголосовали отрицательно.', disabled: true});
        $('.js-vote_plus',karma).attr({title: 'Повысить карму.', disabled: false});
      }else{
        karma.addClass('voted');
      }

      // обновим значение кармы
      $('.js-karma_num', karma).text(json.score);

      // раскрасим positive / negative
      console.log(json.score);
      if(json.score !== '0'){
        $('.js-karma-mark', karma).removeClass('voting-wjt__counter_positive voting-wjt__counter_negative').addClass('voting-wjt__counter_'+json.sign);
      } else {
        $('.js-karma-mark', karma).removeClass('voting-wjt__counter_positive voting-wjt__counter_negative');
      }
    }else{
      show_system_error(json);
    }
  },'json');
  return false;
}


$(document).ready(function(){

  /**
   * Подписаться/отписаться от юзера
   */

  $('#followUser').click(function(){
    var link = $(this);
        link.addClass('loading');
    var login = $(this).attr('data-login');
    $.post('/json/users/follow/', {'login':login}, function(json){
      if(json.messages =='ok'){
        $('#unfollowUser').removeClass('hidden');
        $('#followUser').addClass('hidden');
        var followers_count_string = '<a href="/users/'+login+'/subscription/followers/">'+json.followers_count_string+'</a>';
        $('#followers_count').html(followers_count_string);
      }else{
        show_system_error(json);
      }
      link.removeClass('loading');
    },'json');
    return false;
  });

  $('#unfollowUser').click(function(){
    var login = $(this).attr('data-login');
    var link = $(this);
        link.addClass('loading');

      $.post('/json/users/unfollow/', {'login':login}, function(json){
        if(json.messages =='ok'){
          $('#unfollowUser').addClass('hidden');
          $('#followUser').removeClass('hidden');
          var followers_count_string = json.followers_count > 0 ? '<a href="/users/'+login+'/subscription/followers/">'+json.followers_count_string+'</a>' : json.followers_count_string;
          $('#followers_count').html(followers_count_string);
        }else{
          show_system_error(json);
        }
        link.removeClass('loading');
      },'json');

    return false;
  });


  // обработчик кнопки "подарить приглашение"
  $('#give_invite_button').click(function(){
    var login = $(this).data('login');
    $.post('/json/users/send_invite/', {login: login}, function(json){
      if(json.messages == 'ok'){

        $.jGrowl('Вы подарили приглашение.<br> У вас осталось '+json.invites_string+'.', { theme: 'notice' });
        ajaxFormRedirect(json);
      }else{
        show_system_error(json);
      }
    });
    return false;
  });

  // кнопка "подарить приглашение"
  $('#send_invite').click(function(){
    var user_id = $(this).data('user_id');
    $.post('/json/users/set_invite/', { 'user_id': user_id }, function(json){
      if(json.messages == 'ok'){
        $.jGrowl('Приглашение отправлено', { theme: 'notice' });
      }else{
        show_system_error(json);
      }
    }, 'json');
    return false;
  });

  // кнопка "пройти ппг"
  $('#go_ppg').click(function(){
    var user_id = $(this).data('user_id');
    $.post('/json/users/ppg_comments/', { 'user_id': user_id }, function(json){
      if(json.messages == 'ok'){
        $.jGrowl('Пользователь отправлен на ППГ', { theme: 'notice' });
      }else{
        show_system_error(json);
      }
    }, 'json');
    return false;
  });


  // кнопка "пройти ПИС"
  $('#go_pis').click(function(){
    var user_id = $(this).data('user_id');
    $.post('/json/users/ppg_smiles/', { 'user_id': user_id }, function(json){
      if(json.messages == 'ok'){
        $.jGrowl('Пользователь отправлен на ПИС', { theme: 'notice' });
      }else{
        show_system_error(json);
      }
    }, 'json');
    return false;
  });

  $('.dropdown-toggle').dropdown()

});
