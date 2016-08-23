client 			server
connection----succeed--->connection
  |
  |
 \|/
 Client Ready ---------->get 'Client Ready Message'
                            |
			    |
			    |
			   \|/
 trigger timer  <-------Server Ready
  |
  |
  |
 \|/
 Control Stream------------> handler and send back other data
