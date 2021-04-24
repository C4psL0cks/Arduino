<?php
$timestamp = date("d-m-Y - H:i:s");
$datastring = $_POST["params"];
$myfile = fopen("newfile.txt", "w") or die("Unable to open file!");
$txt = $timestamp . " - ".$datastring."\n";
fwrite($myfile, $txt);
fclose($myfile);
echo $txt;
?>