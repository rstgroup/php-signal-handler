--TEST--
Check properly working attach_signal function
--SKIPIF--
<?php if (!extension_loaded("signal_handler")) print "skip extension signal_handler not loaded"; ?>
<?php if (!extension_loaded("posix")) print "skip posix extension not available"; ?>
<?php if (substr(PHP_OS, 0, 3) == 'WIN') die("skip extension won't work on Windows system"); ?>
--FILE--
<?php

function getInfo($noStatus){
	echo "getInfo called with status $noStatus\n";
}

$r = attach_signal(10, 'getInfo');

posix_kill(posix_getpid(), 10);
var_dump($r);

?>
--EXPECT--
getInfo called with status 10
bool(true)