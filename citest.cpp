int main (int argc, const char **argv)
{
    if (argc == 1) {
	return 0;
    }

    if (argv[1][0] == '1') {
	return 1;
    }

    return -1;
}

// Local Variables:
// tab-width: 8
// mode: C++
// c-basic-offset: 4
// indent-tabs-mode: t
// c-file-style: "stroustrup"
// End:
// ex: shiftwidth=4 tabstop=8