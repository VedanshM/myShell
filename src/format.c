int rem_extra_spaces(char *s) {
	int x = 0;
	int spcnt = 0;
	for (int i = 0; s[i]; i++) {
		if (s[i] == '\t')
			s[i] = ' ';
		if (s[i] != ' ' || (x > 0 && s[x - 1] != ' ')) {
			s[x++] = s[i];
			if (s[x - 1] == ' ')
				spcnt++;
		}
	}
	if (s[x - 1] != ' ')
		x++;
	else
		spcnt--;
	s[x - 1] = 0;
	return spcnt;
}
