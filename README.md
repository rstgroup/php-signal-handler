PHP Signal Handler
====================

Extension uses C <b>signal</b> function (from <signal.h>) to call previously defined callback, if appropriate signal has arrived (i.e SIGUSR1).

### When should I use it?

Could be used with <b>php-amqp</b> (https://github.com/pdezwart/php-amqp) to print actual worker/consumer status (ie. message stats, last operation performed etc.) <br />
What's more it is possible to "catch" unix signal i.e. SIGTERM and then kill the process nicely (program won't stop untill all operations are completed). It is very useful if script operates on sensitive data which cannot be damaged.

### Why don't you use pcntl_signal?
If blocked method is being perfromed (such as consume in php-amqp) then it's not possible to run callback function by calling <i>pcntl_signal_dispatch();</i>. Simply it will not work becuse the script is being blocked by that method.

### Compatibility
Extension will work only where C function signal is available i.e UNIX systems.<br/>
PHP versions supported: <br/>
* PHP 5.4
* PHP 5.3
* PHP 5.2

### Example
```php
<?php

function getInfo($noStatus){
	echo "getInfo called with status $noStatus\n";
}

$r = attach_signal(10, 'getInfo');
while(true){}

?>
```

Spawn the process:
```
php your_script_name.php
```

Kill the first process from the list:
```
kill -s SIGUSR1 `ps -ef | grep php | head -n1 | awk '{print $2}'`
```

After that steps you should see:
```
getInfo called with status 10\n
```
