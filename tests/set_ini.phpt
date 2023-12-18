--TEST--
Test pib_guess
--SKIPIF--
<?php if (!extension_loaded("test_pib")) print "skip"; ?>
--FILE--
<?php
var_dump(ini_set('test_pib.rnd_max', 10));
var_dump(ini_get('test_pib.rnd_max'));
?>
--EXPECTF--
string(3) "100"
string(2) "10"
