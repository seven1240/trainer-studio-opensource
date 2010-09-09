;;;;

// Flash callback
function idp_xx_movie_DoFSCommand(cmd, args) {
//    alert(cmd);
    if ("undefined" != typeof mainWindow) {
        mainWindow.onFSCommand(cmd, args);
    }else{
        alert("no mainwindow");
    }
}

// If the movie already there
var obj;
if( obj = document.getElementById("idp_xx_movie") ) {
    //found
    obj.FlashVars = vars;
    obj.LoadMovie(0, url);
    return;
}

// Create movie
var t = '';
t += '<embed name="idp_xx_movie" id="idp_xx_movie" src="' + url + '" ';
t += 'quality="high" bgcolor="" wmode="opaque" ';
t += 'width="100%" height="100%" name="interaction" align="middle" ';
t += 'play="true" loop="false"  allowScriptAccess="sameDomain" type="application/x-shockwave-flash" ';
t += 'pluginspage="http://www.adobe.com/go/getflashplayer" ';
t += 'flashvars="' + vars + '"></embed>';

document.write(t);
