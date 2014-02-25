--TEST--
Check errors in attach_signal function
--INI--
display_errors=0
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
var_dump($r);

$r = detach_signal(44);
echo $php_errormsg."\n";

$r = detach_signal(11);
echo $php_errormsg;

?>
--EXPECT--
bool(true)
detach_signal(): Invalid signal
detach_signal(): Cannot detach signo: 11