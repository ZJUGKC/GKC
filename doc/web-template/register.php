<?php
session_start();

$name = '';
$pwd = '';
$repwd = '';
$email = '';

//flags
$session_flag = true;
$timeout_flag = false;
$complete_flag = true;
$confirm_flag = true;

$conn_flag = true;
$query_flag = true;
$user_flag = true;
$insert_flag = true;

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

	//variables
	$name = $_POST['user'];
	$pwd = $_POST['pwd1'];
	$repwd = $_POST['pwd2'];
	$email = $_POST['email'];

	//input
	if( empty($name) || empty($pwd) || empty($repwd) || empty($email) ) {
		$complete_flag = false;
	}
	elseif( $pwd != $repwd ) {
		$confirm_flag = false;
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
				if( pg_num_rows($result) > 0 ) {
					$user_flag = false;
				}
				else {
					//insert
					$query_string = "INSERT INTO $$TABLE-NAME$$($$FIELD-USER$$, $$FIELD-PWD$$, $$FIELD-ADDRESS$$) values('$name', '$pwd', '$email');";
					$result = pg_query($dbconn, $query_string);
					if( !$result ) {
						$insert_flag = false;
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
<?php elseif( $complete_flag == false ) : ?>
$$INCOMPLETE_TITLE$$
<?php elseif( $confirm_flag == false ) : ?>
$$MISMATCH_TITLE$$
<?php elseif( $conn_flag == false ) : ?>
$$CONNECT_FAILED_TITLE$$
<?php elseif( $query_flag == false ) : ?>
$$QUERY_FAILED_TITLE$$
<?php elseif( !$user_flag ) : ?>
$$USER_EXISTS_TITLE$$
<?php elseif( !$insert_flag ) : ?>
$$INSERT_ERROR_TITLE$$
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
<?php elseif( $complete_flag == false ) : ?>
<h1>The input information is incomplete!</h1>
<p><a href="admin.php?<?php echo htmlspecialchars(SID); ?>">Return to administrative page!</a></p>
<?php elseif( $confirm_flag == false ) : ?>
<h1>The password confirmation does not match!</h1>
<p><a href="admin.php?<?php echo htmlspecialchars(SID); ?>">Return to administrative page!</a></p>
<?php elseif( $conn_flag == false ) : ?>
<h1>Cannot connect the database!</h1>
<p><a href="admin.php?<?php echo htmlspecialchars(SID); ?>">Return to administrative page!</a></p>
<?php elseif( $query_flag == false ) : ?>
<h1>Query failed!</h1>
<p><a href="admin.php?<?php echo htmlspecialchars(SID); ?>">Return to administrative page!</a></p>
<?php elseif( !$user_flag ) : ?>
<h1>User exists!</h1>
<p><a href="admin.php?<?php echo htmlspecialchars(SID); ?>">Return to administrative page!</a></p>
<?php elseif( !$insert_flag ) : ?>
<h1>Insert failed!</h1>
<p><a href="admin.php?<?php echo htmlspecialchars(SID); ?>">Return to administrative page!</a></p>
<?php else : ?>
<h1>Registration is successful!</h1>
<p><a href="admin.php?<?php echo htmlspecialchars(SID); ?>">Return to administrative page!</a></p>
<?php endif; ?>

</body>

</html>
