--TEST--
Test fahrenheit_to_celsius_ref
--SKIPIF--
<?php if (!extension_loaded("test_pib")) print "skip"; ?>
--FILE--
<?php
$ref = 70;
fahrenheit_to_celsius_ref($ref);
printf("%.2f", $ref);
?>
--EXPECTF--
21.11