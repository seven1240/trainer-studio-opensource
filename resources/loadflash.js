function movie_DoFSCommand(cmd, args) {
    alert('aaa'); mainWindow.onFSCommand(cmd, args);
}

var t = '';
t += '<embed name="movie" id="movie" src="' + url + '" ';
t += 'quality="high" bgcolor="" wmode="opaque" ';
t += 'width="100%" height="100%" name="interaction" align="middle" ';
t += 'play="true" loop="false"  allowScriptAccess="sameDomain" type="application/x-shockwave-flash" ';
t += 'pluginspage="http://www.adobe.com/go/getflashplayer" ';
t += 'flashvars="' + vars + '"></embed>';
//t += '<script language="javascript">function movie_DoFSCommand(cmd, args){mainWindow.onFSCommand(cmd, args);}</script>';

alert(vars);
document.write(t);
