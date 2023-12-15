--TEST--
Test multiple_fahrenheit_to_celsius
--SKIPIF--
<?php if (!extension_loaded("test_pib")) print "skip"; ?>
--FILE--
<?php
$celsius = multiple_fahrenheit_to_celsius([70]);
printf("%.2f", $celsius[0]);
?>
--EXPECTF--
21.11
