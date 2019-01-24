<?php
session_start();
$_SESSION['time'] = time();
session_regenerate_id();

//variables
$name = $_POST['user'];
$pwd = $_POST['pwd'];

//flags
$complete_flag = true;

$conn_flag = true;
$query_flag = true;
$pwd_flag = true;
$admin_flag = false;

//check
if( empty($name) || empty($pwd) ) {
	$complete_flag = false;
}
else {
	//database
	$conn_string = "host=$$localhost$$ port=5432 dbname=$$DB-NAME$$ user=$$DB-USER$$ password=$$DB-PWD$$";
	$dbconn = pg_connect($conn_string);
	if( $dbconn ) {
		//query
		$query_string = "SELECT * FROM $$TABLE-NAME$$ WHERE $$FIELD-USER$$='$name';";
		$result = pg_query($dbconn, $query_string);
		if( $result ) {
			if( pg_num_rows($result) != 1 ) {
				$query_flag = false;
			}
			else {
				//row
				$arr = pg_fetch_array($result, 0);
				if( $arr['$$FIELD-PWD-LOWERCASE$$'] == $pwd ) {
					if( $name == '$$ADMIN-NAME$$' ) {
						$admin_flag = true;
					}
				}
				else {
					$pwd_flag = false;
				}
			}
		}
		else {
			$query_flag = false;
		}
		//close
		pg_close($dbconn);
	}
	else {
		$conn_flag = false;
	}
}

//admin
if( $admin_flag ) {
	header("Location: admin.php?" . htmlspecialchars(SID));
	exit;
}
?>

<!DOCTYPE html>
<html>

<head>
<meta charset="UTF-8">
<title>
<?php if( $complete_flag == false ) : ?>
$$INCOMPLETE_TITLE$$
<?php elseif( $conn_flag == false ) : ?>
$$CONNECT_FAILED_TITLE$$
<?php elseif( $query_flag == false ) : ?>
$$QUERY_FAILED_TITLE$$
<?php elseif( !$pwd_flag ) : ?>
$$PWD_ERROR_TITLE$$
<?php else : ?>
$$TITLE$$
<?php endif; ?>
</title>
</head>

<body>

<?php if( $complete_flag == false ) : ?>
<h1>The input information is incomplete!</h1>
<p><a href="index.htm">Return to log in page!</a></p>
<?php elseif( $conn_flag == false ) : ?>
<h1>Cannot connect the database!</h1>
<p><a href="index.htm">Return to log in page!</a></p>
<?php elseif( $query_flag == false ) : ?>
<h1>Query failed!</h1>
<p><a href="index.htm">Return to log in page!</a></p>
<?php elseif( !$pwd_flag ) : ?>
<h1>Error password!</h1>
<p><a href="index.htm">Return to log in page!</a></p>
<?php else : ?>
<h1><?php echo "User: " . $name; ?></h1>
$$USER_PAGE$$
<p><a href="index.htm">Log out!</a></p>
<?php endif; ?>

</body>

</html>
