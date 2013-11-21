<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html lang="EN" dir="ltr" xmlns="http://www.w3.org/1999/xhtml">
  <head>
	<meta http-equiv="content-type" content="text/xml; charset=utf-8" />
	<title>Table.php</title>
	<style type = "text/css">
	  table, th, td {
		border: 1px solid black;
	  }		 
	</style>
  </head>

  <body>
	<h1>Arduino Yun pin outputs</h1>
  	<?php
		$db=new SQLite3("/mnt/sda1/arduino/www/test/test1.db");
	 	$result = $db->query('SELECT * FROM testdata');
	 	$cols = $result->numColumns();
		print "	 <table> \n";
		print "		 <tr> \n";
		for ($i = 0; $i < $cols; $i++) { 
				print "		   <th>".$result->columnName($i)."</th>	 \n"; 
 		}//end for
		print "		 </tr> \n";

		while ($row = $result->fetchArray()){
			print "		 <tr> \n";
			for ($i = 0; $i < $cols; $i++) { 
			   print "		  <td>".$row[$i] . "</td> \n";
			} //end for 
			print "		 </tr> \n";
		} // end while loop

		print "	   </table> \n";	
  	?>
  </body>
</html>
