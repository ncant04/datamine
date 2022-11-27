#include <iostream>
#include <fstream>
#include <string>

using namespace std;

static void report(const char *fmt, va_list params)
{
	vfprintf(stderr, fmt, params);
	cerr << "Usage: dmine [OPTION] [FILE] [KEYWORD]\n";
}

static void die(const char *fmt, ...)
{
	va_list params;

	va_start(params, fmt);
	report(fmt, params);
	va_end(params);
	exit(1);
}

int main(int argc, char **argv)
{
	int i;
	unsigned int total = 0, found = 0;
	const char *filename = NULL, *keyword = NULL;
	ifstream ifs;
	string temp;
	bool show_results = false;

	if (argc < 3) {
		die("Too few arguments.\n");
	}

	for (i = 1; i < argc; i++) {
		const char *arg = argv[i];

		if (*arg == '-') {
			switch (arg[1]) {
			case 's':
				show_results = true;
				continue;
			}
			die("Unknown argument '%s'\n", arg);
		}

		if (!filename) {
			filename = arg;
			continue;
		}

		if (!keyword) {
			keyword = arg;
			continue;
		}

		die("Too many arguments (%s)\n", arg);
	}

	ifs.open(filename, ios_base::in);

	if (!ifs.is_open()) {
		die("Unable to read file %s (%s)\n", filename, strerror(errno));
	}

	while (ifs.good())
	{
		ifs >> temp;

		if (ifs.good()) {
			total++;
		}

		if (temp.find(keyword) != string::npos)
		{
			if (show_results) {
				cout << temp << endl;
			}

			found++;
		}
	}

	// Print statistics:

	cout << "\n";
	cout << "Total: " << total << endl;
	cout << "Found: " << found << endl;
	cout << "Pct.: " << 100 * double(found) / double(total) << "%\n";

	exit(0);
}
