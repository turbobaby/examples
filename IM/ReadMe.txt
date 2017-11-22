Renan Santana
PID: 4031451
DUE NOV 13,2014

1. User names can only 16 characters

2. Server uses two hash tables: 
		First one is to store the peer sockets.
		Second one is to store the names.
		
		The size of the hash tables are 199.
		Since this is a small program I took the
		approach of using hash tables. 
		I believe there won't be any
		colisions until the ammount of users goes
		pass ~90 users. If any case it would be 
		easy enough to just resize the table, but 
		again this is a small test.
		
3. I interprepted "msg @ user-name" like this
	msg is the message itself sent by the user
	and the user-name is the usernames that 
	the user wants to message to.
	
4. The server displays helpful messages:
	
	a)This output appears when a new user 
	arrives at the server aka joins.
		NEW USER: user1
	
	b)This output appears when a new user 
	arrives at the server but the name
	exists already.
		DUPLICATE USER: user1
	
	c)This output appears when a specific
	user is not connected.
		USER IS NOT CONNECTED: user123
	
	d)This output appears when a user 
	leaves.
		LEAVE: user1
	
	e)This output appears when a new msg 
	arrives at the server.
		INCOMING MSG: hello @ all
		
	f)This output appears when the server
	is ready to send.	
		FROM: user1 / TO: user2 / SEND MSG: hello @ user1
		
	g)Under some certain abnormality the
	server will display this:
		ODD RESULT > USER IS NOT CONNECTED: user1
	This can only happen if the user didn't
	quit in a orderly fashion.
	
5. The client displays helpful messages:
	
	a)When someone leaves the client will
	prompt you who left, 
	the ammount of users connected, and
	all the users that are still connected.
		@ leave  > user2
		@ NUMBER OF USERS CONNECTED : 1 
		@ ONLINE : user1

	b)When someone joins the client will
	prompt you who joined, 
	the ammount of users connected, and
	all the users that are still connected.
		@ join  > user2
		@ NUMBER OF USERS CONNECTED : 2
		@ ONLINE : user1 user2
		
	c)When the user is inputting and misses
	the colon after the last user name this
	message will be displayed:
		@ Missing colon (:). after inputting user name(s).
		
	d)When the user is inputting and enters
	more than 10 users this message will be displayed:
		@ Client can only send to 10 specific users.
		
	e)If the user is trying to send to someone that
	is not in the connected members list this messaage
	will be displayed.
		@ user123: User is not connected
		
	f)When choosing a user-name and you receive this:
		>> user1 << already exist. Please choose a different one.
	Then that means thers a  duplicate and you need to
	choose a defferent user-name.
	
	g)The program will prompt you this:
		Connection to server terminated. Bye now!
	when the program has terminated.
	
6. a)When the user didn't input something 
	valid to send it will prompt you 
	with a error. So you will be stuck
	there until you input some thing valid.
	It's just the way I designed it.
	
	Note: Now if you are still in this loop
	it will not show you any "new" or "leave"
	users until you finished with the msg.
	
	b)Format of input has to be like this:
		user1,..,user10: hello
		or
		user1 user10 user2: hello
	Note that the colon (:) has to be right after
	the name, there cannot be a space then colon.
		
	c)There can only be up to TEN specific users
	that you want to message at a time.
	
	d)The input can be as follows:
		user1,user2:hello
		user1 user2: hello
		quit
		all: hello
		,,./   quit,,.
		
	e)The keyword "all" needs to be by its
	self. 
	Ex.
		all: hello
	If you input:
		user1 all: hello
	The client will prompt you this:
		@ all: User is not connected
	

		
	
