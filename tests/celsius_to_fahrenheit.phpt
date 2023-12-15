--TEST--
Test celsius_to_fahrenheit
--SKIPIF--
<?php if (!extension_loaded("test_pib")) print "skip"; ?>
--FILE--
<?php
printf("%.2f", celsius_to_fahrenheit(30));
?>
--EXPECTF--
86.00