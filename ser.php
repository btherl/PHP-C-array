<?

$arr_outer = array(
  array(
    0 => 'pickles',
    1 => 1,
    2 => 2.5,
    3 => 3.5,
    4 => 'bananas',
  ),
  array(
    0 => null,
    1 => 1,
    2 => 1/3,
    3 => 3.5,
    4 => 'bananas',
  ),
);
print serialize($arr_outer) . "\n";
?>
