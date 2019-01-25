<?php
session_start();

//variables
$user_array = array();

//flags
$session_flag = true;
$timeout_flag = false;

$conn_flag = true;
$query_flag = true;

//check valid
if( empty($_SESSION['userid']) ) {
	$session_flag = false;
}
elseif( $_SESSION['time'] < time() - 3600 ) {
	//timeout
	$timeout_flag = true;
	$_SESSION['userid'] = '';
}
else {
	$_SESSION['time'] = time();  //update

	//database
	$conn_string = "host=$$localhost$$ port=5432 dbname=$$DB-NAME$$ user=$$DB-USER$$ password=$$DB-PWD$$";
	$dbconn = pg_connect($conn_string);
	if( $dbconn ) {
		$query_string = "SELECT * FROM $$TABLE-NAME$$;";
		$result = pg_query($dbconn, $query_string);
		if( $result ) {
			while( $arr = pg_fetch_array($result) ) {
				$user_array[] = $arr['$$FIELD-USER-LOWERCASE$$'];
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
?>

<!DOCTYPE html>
<html>

<head>
<meta charset="UTF-8">
<title>
<?php if( !$session_flag ) : ?>
$$SESSION_INVALID_TITLE$$
<?php elseif( $timeout_flag ) : ?>
$$TIMEOUT_TITLE$$
<?php elseif( $conn_flag == false ) : ?>
$$CONNECT_FAILED_TITLE$$
<?php elseif( $query_flag == false ) : ?>
$$QUERY_FAILED_TITLE$$
<?php else : ?>
$$TITLE$$
<?php endif; ?>
</title>
</head>

<body>

<?php if( !$session_flag ) : ?>
<h1>The current session is invalid!</h1>
<p><a href="index.htm">Return to log in page!</a></p>
<?php elseif( $timeout_flag ) : ?>
<h1>The current session expired!</h1>
<p><a href="index.htm">Return to log in page!</a></p>
<?php elseif( $conn_flag == false ) : ?>
<h1>Cannot connect the database!</h1>
<p><a href="index.htm">Return to log in page!</a></p>
<?php elseif( $query_flag == false ) : ?>
<h1>Query failed!</h1>
<p><a href="index.htm">Return to log in page!</a></p>
<?php else : ?>

<!-- Admin Page -->

<h1>Users</h1>

<table border="1">
<?php for( $i = 0; $i < count($user_array); $i ++ ) : ?>
<tr><td><?php echo $user_array[$i]; ?></td></tr>
<?php endfor; ?>
</table>

<h1>Register</h1>

<form action="register.php?<?php echo htmlspecialchars(SID); ?>" method="post">
<p><input id="user" name="user" type="text" placeholder="user name"/></p>
<p><input id="pwd1" name="pwd1" type="password" placeholder="password"/></p>
<p><input id="pwd2" name="pwd2" type="password" placeholder="confirm password"/></p>
<p><input id="email" name="email" type="email" placeholder="email"/></p>
<p><input id="smt" name="smt" type="submit" placeholder="submit"/></p>
</form>

<h1>Delete</h1>

<form action="delete.php?<?php echo htmlspecialchars(SID); ?>" method="post">
<p><input id="user" name="user" type="text" placeholder="user name"/></p>
<p><input id="smt" name="smt" type="submit" placeholder="submit"/></p>
</form>

<h1>Exit</h1>
<p><a href="index.htm">Log out!</a></p>

<?php endif; ?>

</body>

</html>
