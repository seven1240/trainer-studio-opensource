
var t = '<embed src="' + url + '" ';
t += 'quality="high" bgcolor="" wmode="opaque" ';
t += 'width="100%" height="100%" name="interaction" align="middle" ';
t += 'play="true" loop="false"  allowScriptAccess="sameDomain" type="application/x-shockwave-flash" ';
t += 'pluginspage="http://www.adobe.com/go/getflashplayer" ';
t += 'flashvars="' + vars + '"></embed>';

alert(vars);
document.write("<textarea rows=20 cols=80>");
document.write(t);
document.write("</textarea>");
document.write(t);
