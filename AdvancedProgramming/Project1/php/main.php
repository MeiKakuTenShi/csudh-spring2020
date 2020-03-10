<?php
$myfile = fopen("cycle.c", "r") or die("Unable to open file!");

$line_comment_valid = "/ \/\//";
$line_comment_invalid = "/^ *\/(?!=\/|\*)/";

$multi_comment_start = "/\/\*/";
$multi_comment_end = "/\*\//";

$content = fread($myfile,filesize("cycle.c"));
$sLineComments = array();

$lines = explode("\n", $content);
$result = '';
$errors = array();

for ($i = 0; $i < count($lines);)  {
  if (preg_match($line_comment_valid, $lines[$i]) == 1) {
    $inline = "/[\W\w\s]+(?=\/\/)/";
    if (preg_match($inline, $lines[$i], $matches) == 1)
    $result .= $matches[0];
    // print "line comment: line #" . ($i + 1) . "\n" . $lines[$i] . "\n";
  } elseif (preg_match($line_comment_invalid, $lines[$i]) == 1) {
    print "invalid comment: line #" . ($i + 1) . "\n" . $lines[$i] . "\n";
  } elseif (preg_match($multi_comment_start, $lines[$i]) == 1) {
    $start = $i;
    for(;$start < count($lines);) {
      if(preg_match($multi_comment_end, $lines[$start]) == 1) {
        $i = $start;
        break;
      } else {
        $start++;
      }

      if($start == count($lines)) {
        for($k = $i; $k < count($lines); $k++) {
          if(preg_match($line_comment_valid, $lines[$k]) == 0) {
            $result .= $lines[$k] . "\n";
          }
        }
        print "error: invalid multi-line comment : No End   line #" . ($start + 1) . "\n" . $lines[$i] . "\n";
      }
    }
  } else {
    $result .= $lines[$i] . "\n";
  }
  $i++;
}

print "\n\nFile with valid comments removed: \n\n" . $result;

fclose($myfile);