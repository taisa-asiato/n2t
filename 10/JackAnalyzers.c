#include "define.h"

int gen_inputfilename( struct dirent * dp ) {
	int isjack = 0;
	if ( strstr( dp->d_name, ".jack" ) ) {
		strcpy( streamfilename, dp->d_name );
		isjack = 1;
	}
	return isjack;
}

void gen_outputfilename( char * dirname, char * filename ) {
	char tmpfilename[256];
	char * cp;

	strcpy( tmpfilename, filename );
	cp = strstr( tmpfilename, "." );
	*cp = '\0';
	snprintf( outputfilename, "%s/%s.xml", dirname, tmpfilename );
}

void jack_analyze_main( char ** fname ) {
	DIR * dirp;
	struct dirent * dp;

	if ( ( dirp = opendir( argv[1] ) ) != NULL ) {
		// 入力ファイルがディレクトリの場合
		while ( ( dp = readdir( dirp ) ) != NULL ) {
			// 入力ファイル名を作成
			if ( gen_filename( dp ) ) {
				fprintf( stdout, "%s\n", streamfilename );
				if ( inputfp = fopen( streamfilename, "r"  ) ) {
					gen_outputfilename( fname, streamfilename );
					outputfp = fopen( outputfilename, "w" );
					jack_analyze_main();
					fclose( outputfp );
					fclose( inputfp );
				}
			}
		}
	} else {
		// 入力ファイルがディレクトリでない場合
		strcpy( streamfilename, argv[1] );
		fprintf( stdout, "%s\n", streamfilename );
		if ( fp = fopen( streamfilename, "r"  ) ) {
			gen_outputfilename( fname, streamfilename );
			outputfp = fopen( outputfilename, "w" );
			jack_analyze_main( inputfp, outputfp ); 
			fclose( outputfp );
			fclose( inputfp );
		}
	}
}
