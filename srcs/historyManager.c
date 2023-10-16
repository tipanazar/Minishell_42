// // Function to disable terminal buffering
// void	disableBuffering(void)
// {
// 	struct termios	buf;

// 	tcgetattr(0, &buf);
// 	buf.c_lflag &= ~ICANON;
// 	buf.c_cc[VMIN] = 1;
// 	buf.c_cc[VTIME] = 0;
// 	tcsetattr(0, TCSAFLUSH, &buf);
// }

// // Function to enable terminal buffering
// void	enableBuffering(void)
// {
// 	struct termios	buf;

// 	tcgetattr(0, &buf);
// 	buf.c_lflag |= ICANON;
// 	tcsetattr(0, TCSAFLUSH, &buf);
// }

// void	handleArrows(void)
// {
// 	int	key;

// 	disableBuffering(); // Disable terminal buffering
// 	while (1)
// 	{
// 		key = getchar(); // Read a single character
// 		if (key == 27)
// 		{                   
// 			// The arrow key sequences start with 27 (ASCII escape)
// 			key = getchar(); // Read the second character
// 			if (key == 91)
// 			{                    // The actual arrow key sequences start with 91
// 				key = getchar(); // Read the third character,
// 					 			 //which is the arrow key code
// 				switch (key)
// 				{
// 				case 65:
// 					printf("Up arrow key\n");
// 					break ;
// 				case 66:
// 					printf("Down arrow key\n");
// 					break ;
// 				}
// 			}
// 		}
// 	}
// 	enableBuffering(); // Enable terminal buffering
// }

// void	historyHandler(void)
// {
// 	char buffer[100];

// 	printf("Enter a line of text: ");
// 	if (fgets(buffer, sizeof(buffer), stdin) != NULL)
// 	{
// 		printf("You entered: %s", buffer);
// 	}
// 	else
// 	{
// 		printf("Error reading input.");
// 	}

// 	return (0);
// }