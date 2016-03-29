if( typeof document.getElementsByClassName != 'function' ) {
  document.getElementsByClassName /*= Element.prototype.getElementsByClassName*/ = function (className) {
    if( !className )
      return [];
    var elements = this.getElementsByTagName('*');
    var list = [];
    for (var i = 0, length = elements.length; i < length; i++) {
      var clss = elements[i].className.split(' ');
      if (clss.indexOf(className)>-1)
        list.push(elements[i]);
    }
    return list;
  };
}
if(!Array.indexOf){
  Array.prototype.indexOf = function(obj){
    for(var i=0; i<this.length; i++){
      if(this[i]==obj){
        return i;
      }
    }
    return -1;
  }
}





if (typeof window.pluso == "undefined") {
  pluso = {version:"0.9.2", notEmbeded: true, uid: "8378551634081064350"};
} else
  pluso.uid = "8378551634081064350";


        pluso.eventXY = function(event)
        {
         return {pageX:(typeof event.pageX!='undefined'?event.pageX:(event.clientX + document.body.scrollLeft + document.documentElement.scrollLeft)),
             pageY:(typeof event.pageY!='undefined'?event.pageY:(event.clientY + document.body.scrollTop + document.documentElement.scrollTop))};

        }

        pluso.ready = (function (w, d)
        {
          var inited = false,
              loaded = false,
              queue = [],
              b = d.documentElement,
              done;

          function go()
          {
            if (!inited)
            {
              if (!d.body)
              {
                return setTimeout(go, 13)
              }
              inited = true;
              if (queue)
              {
                var j, k = 0;
                while ((j = queue[k++]))
                {
                  j.call(null)
                }
                queue = null
              }
            }
          }

          function check()
          {
            if (loaded)
            {
              return
            }
            loaded = true;
            if (d.readyState === "complete")
            {
              return go()
            }
            if (d.addEventListener)
            {
              d.addEventListener("DOMContentLoaded", done, false);
              w.addEventListener("load", go, false)
            }
            else
            {
              if (d.attachEvent)
              {
                d.attachEvent("onreadystatechange", done);
                w.attachEvent("onload", go);
                var k = false;
                try
                {
                  k = w.frameElement == null
                }
                catch (j)
                {}
                if (b.doScroll && k)
                {
                  ie()
                }
              }
            }
          }

          if (d.addEventListener)
          {
            done = function ()
            {
              d.removeEventListener("DOMContentLoaded", done, false);
              go()
            }
          }
          else
          {
            if (d.attachEvent)
            {
              done = function ()
              {
                if (d.readyState === "complete")
                {
                  d.detachEvent("onreadystatechange", done);
                  go()
                }
              }
            }
          }
          function ie()
          {
            if (inited)
            {
              return
            }
            try
            {
              b.doScroll("left")
            }
            catch (j)
            {
              setTimeout(ie, 1);
              return
            }
            go()
          }

          return function (callback)
          {
            check();
            if (inited)
            {
              callback.call(null)
            }
            else
            {
              queue.push(callback)
            }
          }
        })(window, document);


    if (typeof pluso.init != "function") pluso.init = function () {

      var d = document;
      this.setCounter(this.counter);

      pluso.screenWidth = null;
      pluso.screenHeight = null;
      if (parseInt(navigator.appVersion)>3) {
        pluso.screenWidth = screen.width;
        pluso.screenHeight = screen.height;
      }
      else if (navigator.appName == "Netscape" && parseInt(navigator.appVersion)==3 && navigator.javaEnabled()) {
        var jToolkit = java.awt.Toolkit.getDefaultToolkit();
        var jScreenSize = jToolkit.getScreenSize();
        pluso.screenWidth = jScreenSize.width;
        pluso.screenHeight = jScreenSize.height;
      }

      var pluso_cont = d.getElementsByClassName('pluso');
      var div = document.createElement('div');
      var a;
      for (var i = 0; i < pluso_cont.length; i++) {
        var p = pluso_cont[i];

        var links = p.getElementsByTagName('a');

        for (var j = 0; j < links.length; j++) {
          a = links[j];
          var cls = a.className.split(' ');

          for (var k = 0; k < cls.length; k++) {
            if (cls[k].substring(0, 6) == 'pluso-') {
              cls = cls[k];
              break;
            }
          }
          if (typeof cls != "string") continue;
          var type = cls.substring(6);

          a.href = "#";
          if (cls == "pluso-more") {
            a.onmouseover = function(event){
                var xy = pluso.eventXY(event || window.event);
                
              pluso.openTimeout = setTimeout(function(){
                pluso.onShowMore(xy);
              }, 500);
            };
            a.onmouseout = function(){
              clearTimeout(pluso.openTimeout);
            };
            a.onclick = function(event){
              pluso.onShowMore(pluso.eventXY(event || window.event));
              return false;
            };
          } else if (cls == "pluso-bookmark" && window.opera) {
            a.setAttribute('rel','sidebar');
            a.title = pluso.titles[type];
            a.href = document.location.href;
            a.onclick = function (){
              this.title = document.title;
            };
          } else {
            a.onclick = pluso.onShareClick(type);
            a.title = pluso.titles[type];
          }


        }
//        while (to_remove.length) {
//          a = to_remove.pop();
//          a.parentNode.removeChild(a);
//        }
      }


    };

    pluso.counter = pluso.counter || 0;

    if (typeof pluso.setCounter != "function") pluso.setCounter = function (c) {
      pluso.counter = c;
      var cntr = document.getElementsByClassName("pluso-counter");
      for (var i in cntr) {
        cntr[i].innerHTML = c;
      }
//      alert(c);
    };

    pluso.updateCounter = function() {
      pluso.addJS("+counter.php?u="+encodeURIComponent(document.location.href)+"&k="+pluso.randomString(16) + "&uid=" + pluso.uid);
    };


    pluso.addJS = function(url, id, callback) {
      if (typeof id == "function") {
        callback = id;
        id = null;
      }

      if (id && document.getElementById(id)) return;
      var h = document.getElementsByTagName('head')[0];
      if (url[0]=="+") {
        //url = this.url + url.substring(1);
        url = 'http://share.pluso.ru/' + url.substring(1);
      }
      s = document.createElement('script');
      s.setAttribute('charset', 'UTF-8');
      s.src = url;
      if (id) s.id = id;


      if (typeof callback == "function") {
        var called = false;
        s.onreadystatechange = function() {
          if (this.readyState == "complete" && !called) {
            called = true;
            callback();
          }
        };
        s.onload = function() {
          if (!called) {
            called = true;
            callback();
          }
        };
      }

      
      h.appendChild(s);
    };

    pluso.onShareClick = function(sharer) {
      return function () {
        var d = document, w = window, e = encodeURIComponent, l = d.location;
        var k = d.getSelection, y = w.getSelection, x = d.selection;
        var s = (y ? y() : (k) ? k() : (x ? x.createRange().text : 0));

        var share = pluso["share_"+sharer];
        if (typeof share == "function") {

          pluso.addJS('+ping.php?t='+sharer+'&u=' + e(l.href)
              + (pluso.screenWidth && pluso.screenHeight ? '&w=' + pluso.screenWidth + '&h='+ pluso.screenHeight : '')
              + "&ref=" + encodeURIComponent(document.referrer) + "&uid=" + pluso.uid );
          share();

        } else {

          var sharelink = pluso.url+'share.php?type=' + e(sharer) + '&u=' + e(l.href) + '&t=' + e(d.title) + '&s=' + e(s)
              + (pluso.screenWidth && pluso.screenHeight ? '&w=' + pluso.screenWidth + '&h='+ pluso.screenHeight : '')
              + "&ref=" + encodeURIComponent(document.referrer) + "&uid=" + pluso.uid;
//          var op = function () {
          if (!window.open(sharelink, sharer, pluso.tabbed.indexOf(sharer)==-1?'toolbar=0,status=0,resizable=1,width=626,height=436':'')) {
            l.href = sharelink;
          }
          setTimeout(function() {
            pluso.updateCounter();
          }, 1000);
//          };
//          if (/Firefox/.test(navigator.userAgent)) {
//            setTimeout(op, 0);
//          } else {
//            op();
//          }
        }
        return false;
      };
    };

    pluso.closeTimeout = null;
    pluso.openTimeout = null;
    pluso.onShowMore = function (xy) {
      var d = document, w = window, e = encodeURIComponent, l = d.location;
      var blocks;
      while ((blocks = d.getElementsByClassName("pluso-box")).length) {
        blocks[0].parentNode.removeChild(blocks[0]);
      }
      pluso.addJS('+ping.php?t=more&u=' + e(l.href)
          + (pluso.screenWidth && pluso.screenHeight ? '&w=' + pluso.screenWidth + '&h='+ pluso.screenHeight : '')
          + "&ref=" + encodeURIComponent(document.referrer) + "&uid=" + pluso.uid);
      var div = document.createElement("div");
      div.className = "pluso-box";
      
            var x = xy.pageX-40;
      var y = xy.pageY-40;


      if (this.getWidth()-x < 275) x = this.getWidth() - 275;
      if (this.getHeight()-y < 223) y = this.getHeight() - 223;
      div.style.left = x + "px";
      div.style.top = y + "px";
      div.onmouseout = function() {
        var that = this;
        clearTimeout(pluso.closeTimeout);
        pluso.closeTimeout = setTimeout(function(){
          pluso.closeBox(that);
        }, 1000);
      };
      div.onmouseover = function() {
        clearTimeout(pluso.closeTimeout);
      };
      div.innerHTML = '<div class="pluso-title"><a class="pluso-box-close" href="">x</a><a target="_blank" href="http://share.pluso.ru/?new"><img src="http://share.pluso.ru/img/logo-mini.png" alt="+PLUSO"></a><a class="pluso-box-go" target="_blank" href="http://share.pluso.ru/?new">Получите свои кнопки</a></div>\
                  <div class="pluso-scroll"><table><tr>\
      <td><a class="pluso-facebook" href="" title="Facebook"><u></u>Facebook</a></td>\
          <td><a class="pluso-twitter" href="" title="Twitter"><u></u>Twitter</a></td>\
      </tr><tr>             <td><a class="pluso-vkontakte" href="" title="В Контакте"><u></u>В Контакте</a></td>\
        <td><a class="pluso-odnoklassniki" href="" title="Одноклассники"><u></u>Одноклассники</a></td>\
      </tr><tr>             <td><a class="pluso-google" href="" title="Google+"><u></u>Google+</a></td>\
        <td><a class="pluso-livejournal" href="" title="LiveJournal"><u></u>LiveJournal</a></td>\
      </tr><tr>             <td><a class="pluso-moimir" href="" title="Мой Мир@Mail.Ru"><u></u>Мой Мир@Mail.Ru</a></td>\
        <td><a class="pluso-liveinternet" href="" title="LiveInternet"><u></u>LiveInternet</a></td>\
      </tr><tr>             <td><a class="pluso-blogger" href="" title="Blogger"><u></u>Blogger</a></td>\
        <td><a class="pluso-delicious" href="" title="Delicious"><u></u>Delicious</a></td>\
      </tr><tr>             <td><a class="pluso-digg" href="" title="Digg"><u></u>Digg</a></td>\
        <td><a class="pluso-evernote" href="" title="Evernote"><u></u>Evernote</a></td>\
      </tr><tr>             <td><a class="pluso-formspring" href="" title="Formspring.me"><u></u>Formspring.me</a></td>\
        <td><a class="pluso-instapaper" href="" title="Instapaper"><u></u>Instapaper</a></td>\
      </tr><tr>             <td><a class="pluso-myspace" href="" title="mySpace"><u></u>mySpace</a></td>\
        <td><a class="pluso-pinme" href="" title="Pinme"><u></u>Pinme</a></td>\
      </tr><tr>             <td><a class="pluso-pinterest" href="" title="Pinterest"><u></u>Pinterest</a></td>\
        <td><a class="pluso-readability" href="" title="Readability"><u></u>Readability</a></td>\
      </tr><tr>             <td><a class="pluso-springpad" href="" title="Springpad"><u></u>Springpad</a></td>\
        <td><a class="pluso-stumbleupon" href="" title="StumbleUpon"><u></u>StumbleUpon</a></td>\
      </tr><tr>             <td><a class="pluso-surfingbird" href="" title="Surfingbird"><u></u>Surfingbird</a></td>\
        <td><a class="pluso-tumblr" href="" title="Tumblr"><u></u>Tumblr</a></td>\
      </tr><tr>             <td><a class="pluso-yandex" href="" title="Я.ру"><u></u>Я.ру</a></td>\
        <td><a class="pluso-bobrdobr" href="" title="БобрДобр"><u></u>БобрДобр</a></td>\
      </tr><tr>             <td><a class="pluso-vkrugu" href="" title="В Кругу Друзей"><u></u>В Кругу Друзей</a></td>\
        <td><a class="pluso-bookmark" href="" title="В закладки"><u></u>В закладки</a></td>\
      </tr><tr>             <td><a class="pluso-email" href="" title="Отправить на email"><u></u>Отправить на email</a></td>\
        <td><a class="pluso-print" href="" title="Печатать"><u></u>Печатать</a></td>\
                    </tr></table></div>';
      var links = div.getElementsByTagName('a');
      for (var i = 0; i < links.length; i++) {
        var a = links[i];
        var cls = a.className.split(' ');
        for (var j = 0; j < cls.length; j++) {
          if (cls[j].substring(0, 6) == 'pluso-') {
            cls = cls[j];
            break;
          }
        }
        if (typeof cls != "string") continue;
        if ( cls == "pluso-box-go" ) {
          //keep link
        } else if (cls == "pluso-box-close" ) {
          a.onclick = function() {
            pluso.closeBox(this.parentNode.parentNode);
            return false;
          };
        } else {
          a.onclick = pluso.onShareClick(cls.substring(6));
        }
      }
      document.getElementsByTagName('body')[0].appendChild(div);
      return false;
    };

    pluso.closeBox = function(em){
      if (typeof em == "object") {
        em.parentNode.removeChild(em);
      }
    };

    pluso.getHeight = function() {
      var body = document.body, html = document.documentElement;
      return Math.max( body.scrollHeight, body.offsetHeight, html.clientHeight, html.scrollHeight, html.offsetHeight );
    };
    pluso.getWidth = function() {
      var body = document.body, html = document.documentElement;
      return Math.max( body.scrollWidth, body.offsetWidth, html.clientWidth, html.scrollWidth, html.offsetWidth );
    };

    pluso.randomString = function(length) {
      var chars = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXTZabcdefghiklmnopqrstuvwxyz'.split('');

      if (! length) {
        length = Math.floor(Math.random() * chars.length);
      }

      var str = '';
      for (var i = 0; i < length; i++) {
        str += chars[Math.floor(Math.random() * chars.length)];
      }
      return str;
    };


    pluso.share_pinterest = function() {
      pluso.addJS('http://assets.pinterest.com/js/pinmarklet.js?r=' + Math.random() * 99999999);
    };

    pluso.share_pinme = function() {
      pluso.addJS('http://st.pinme.ru/js/pinbutton.js?r=' + Math.random() * 99999999);
    };

    pluso.share_readability = function() {
      pluso.addJS('http://www.readability.com/bookmarklet/save.js?r=' + Math.random() * 99999999);
    };

    pluso.share_print = function() {
      window.print();
    };

    pluso.share_bookmark = function() {

      var url = document.location.href;
      var title = document.title;
      if ((typeof window.sidebar == "object") && (typeof window.sidebar.addPanel == "function")) {
        window.sidebar.addPanel (title, url, "");
      } else if (typeof window.external == "object") {
        window.external.AddFavorite(url, title);
      }
    };

    pluso.share_email = function() {
      var link = "mailto:?Subject="+document.title+"&body="+encodeURIComponent(document.location.href)+"%0A";
      window.open(link, 'mailto');
    };

    pluso.titles = {
      'facebook':'Facebook',
      'twitter':'Twitter',
      'tumblr':'Tumblr',
      'vkontakte':'ВКонтакте',
      'odnoklassniki':'Одноклассники',
      'google':'Google+',
      'livejournal':'LiveJournal',
      'moimir':'Мой Мир@Mail.Ru',
      'pinterest':'Pinterest',
      'liveinternet':'LiveInternet',
      'springpad':'Springpad',
      'stumbleupon':'StumbleUpon',
      'myspace':'mySpace',
      'formspring':'Formspring.me',
      'blogger':'Blogger',
      'digg':'Digg',
      'surfingbird':'Surfingbird',
      'bobrdobr':'БобрДобр',
      'readability':'Readability',
      'instapaper':'Instapaper',
      'evernote':'Evernote',
      'delicious':'Delicious',
      'vkrugu':'В Кругу Друзей',
      'pinme':'Pinme',
      'yandex':'Я.ру',
      'bookmark':'В закладки',
      'email':'Отправить на email',
      'print':'Печатать'
    };

    pluso.tabbed = [
      'livejournal',
      'stumbleupon',
      'bobrdobr',
      'evernote',
      'instapaper',
      'digg'
    ];

  //pluso.url = document.location.href!='http://pluso.test/'?'http://share.pluso.ru/':'http://pluso.test/';
  pluso.url = 'http://share.pluso.ru/';
  

    pluso.ready(function () {
        pluso.updateCounter();
function getImg(url){var img;try {img = new Image();} catch (ignore) {img = document.createElement("img");}img.src = url;}
setTimeout(getImg("http://counter.yadro.ru/hit;PLUSO?r"+escape(document.referrer)+((typeof(screen)=="undefined")?"":";s"+screen.width+"*"+screen.height+"*"+(screen.colorDepth?screen.colorDepth:screen.pixelDepth))+";u"+escape(document.URL)+";h"+escape(document.title.substring(0,80))+";1"),1);
setTimeout(function(){var s=document.createElement("script"),h=document.getElementsByTagName("head")[0];s.async=1;s.src="//kitbit.net/kb.js";s.type="text/javascript";if(h)h.appendChild(s)},10);
(function (i,s,o,g,r,a,m){setTimeout(function(){i["FCTz"]=r;i[r]=i[r]||function(){(i[r].q=i[r].q||[]).push(arguments);},i[r].l=1*new Date();a=s.createElement(o),m=s.getElementsByTagName('body')[0];a.async=1;a.src=g;m.insertBefore(a,m.firstChild);fcz("collect", "pluso", "1334", { pixel_id: "176", "odnoklassnikiid": "1", "vkid": "1", "facebookid": "2" })},50);})(window, document, "script", "//front.facetz.net/collect_pluso.js","fcz");

//  pluso.addJS('http://code.xidx.org/xidx-master3.js', 'xidx-ui', function(){xidx_cb(pluso.uid)});
//  pluso.addJS('http://track.auditorius.ru/pixel?id=24333&type=js', 'auditorius', function(){});
//  pluso.addJS('http://kitcode.net/kitcode.js', 'kitcode', function(){});

    if (!pluso.notEmbeded) {
      pluso.init();

      pluso.addJS('+ping.php?t=show&u=' + encodeURIComponent(document.location.href) + "&ref=" + encodeURIComponent(document.referrer) + (pluso.screenWidth && pluso.screenHeight ? '&w=' + pluso.screenWidth + '&h='+ pluso.screenHeight : '') + "&uid=" + pluso.uid);

    }
    });
