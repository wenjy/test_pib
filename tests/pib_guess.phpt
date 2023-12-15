--TEST--
Test pib_guess
--SKIPIF--
<?php if (!extension_loaded("test_pib")) print "skip"; ?>
--FILE--
<?php
echo pib_guess(101);
?>
--EXPECTF--
less