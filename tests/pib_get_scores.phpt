--TEST--
Test pib_get_scores
--SKIPIF--
<?php if (!extension_loaded("test_pib")) print "skip"; ?>
--FILE--
<?php
pib_guess(101);
var_dump(pib_get_scores());
?>
--EXPECTF--
array(0) {
}
