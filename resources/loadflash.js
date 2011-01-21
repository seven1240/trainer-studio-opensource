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


// Create movie
var t = '';
t += '<embed name="idp_xx_movie" id="idp_xx_movie" src="' + url + '" ';
t += 'width="100%" height="100%" ';
t += 'type="application/x-shockwave-flash" ';
t += 'flashvars="' + vars + '"></embed>';

document.close();
// document.write will open a new document
document.write(t);
