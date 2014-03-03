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

$r = attach_signal(33, 'getInfo');
echo $php_errormsg."\n";

$r = attach_signal(11, 'getNonExistingCallback');
echo $php_errormsg."\n";

$r = attach_signal(11);
echo $php_errormsg;

?>
--EXPECT--
attach_signal(): Invalid signal
attach_signal(): getNonExistingCallback is not a callable function name error
attach_signal() expects exactly 2 parameters, 1 given
