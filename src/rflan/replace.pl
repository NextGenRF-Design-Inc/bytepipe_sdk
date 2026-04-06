#!/usr/bin/perl
use strict;
use warnings;
use File::Path qw(make_path);

# 1. Define the file path
my ($input_path, $output_path) = @ARGV;


# 2. Read the entire file into a single string
open my $fh, '<', $input_path or die "Could not open $input_path: $!";
my $content = do { local $/; <$fh> };
close $fh;

# 3. Define the exact text to find
my @find_lines1 = (
    'uint8_t* stream_binary_STANDARD_BYTES_252;',
    'FILE *stream_binary_STANDARD_BYTES_252_FILE = fopen("stream_binary_STANDARD_BYTES_252.bin", "rb");',
	'fseek(stream_binary_STANDARD_BYTES_252_FILE, 0, SEEK_END);',
	'int64_t stream_binary_STANDARD_BYTES_252_FILE_SIZE = ftell(stream_binary_STANDARD_BYTES_252_FILE);',
	'fseek(stream_binary_STANDARD_BYTES_252_FILE, 0, SEEK_SET);',
	'stream_binary_STANDARD_BYTES_252 = (uint8_t *) malloc(stream_binary_STANDARD_BYTES_252_FILE_SIZE * sizeof(uint8_t));',
	'fread(stream_binary_STANDARD_BYTES_252, sizeof(uint8_t), stream_binary_STANDARD_BYTES_252_FILE_SIZE, stream_binary_STANDARD_BYTES_252_FILE);',
	'fclose(stream_binary_STANDARD_BYTES_252_FILE);'
);

my @find_lines2 = (
	'uint8_t* arm_binary_STANDARD_BYTES_252;',
	'FILE *arm_binary_STANDARD_BYTES_252_FILE = fopen("arm_binary_STANDARD_BYTES_252.bin", "rb");',
	'fseek(arm_binary_STANDARD_BYTES_252_FILE, 0, SEEK_END);',
	'int64_t arm_binary_STANDARD_BYTES_252_FILE_SIZE = ftell(arm_binary_STANDARD_BYTES_252_FILE);',
	'fseek(arm_binary_STANDARD_BYTES_252_FILE, 0, SEEK_SET);',
	'arm_binary_STANDARD_BYTES_252 = (uint8_t *) malloc(arm_binary_STANDARD_BYTES_252_FILE_SIZE * sizeof(uint8_t));',
	'fread(arm_binary_STANDARD_BYTES_252, sizeof(uint8_t), arm_binary_STANDARD_BYTES_252_FILE_SIZE, arm_binary_STANDARD_BYTES_252_FILE);',
	'fclose(arm_binary_STANDARD_BYTES_252_FILE);'
);

# 3. Escape each line automatically and join with flexible whitespace
# quotemeta() adds the backslashes for you!
my $pattern1 = join('\s*\n?\s*', map { quotemeta($_) } @find_lines1);
my $pattern2 = join('\s*\n?\s*', map { quotemeta($_) } @find_lines2);
	
# 4. Define the multi-line replacement text
my $replace1 = <<'EOF';
	
	
	FRESULT fr;
	UINT br; // Bytes read
	FIL stream_binary_STANDARD_BYTES_252_FILE;
	int64_t stream_binary_STANDARD_BYTES_252_FILE_SIZE;
	uint8_t* stream_binary_STANDARD_BYTES_252;
	fr = f_open(&stream_binary_STANDARD_BYTES_252_FILE, "stream_binary_STANDARD_BYTES_252.bin", FA_OPEN_EXISTING | FA_READ);
	if( fr == FR_OK )
	{
		stream_binary_STANDARD_BYTES_252_FILE_SIZE = f_size(&stream_binary_STANDARD_BYTES_252_FILE);
	    stream_binary_STANDARD_BYTES_252 = (uint8_t*) pvPortMalloc(sizeof(uint8_t) * stream_binary_STANDARD_BYTES_252_FILE_SIZE);
	    if (stream_binary_STANDARD_BYTES_252 != NULL) {
	        f_read(&stream_binary_STANDARD_BYTES_252_FILE, stream_binary_STANDARD_BYTES_252, stream_binary_STANDARD_BYTES_252_FILE_SIZE, &br);
	    }
	    f_close(&stream_binary_STANDARD_BYTES_252_FILE);
	}
EOF

my $replace2 = <<'EOF';
	
	
	FIL arm_binary_STANDARD_BYTES_252_FILE;
	int64_t arm_binary_STANDARD_BYTES_252_FILE_SIZE;
	uint8_t* arm_binary_STANDARD_BYTES_252;
	fr = f_open(&arm_binary_STANDARD_BYTES_252_FILE, "arm_binary_STANDARD_BYTES_252.bin", FA_OPEN_EXISTING | FA_READ);
	if( fr == FR_OK )
	{
		arm_binary_STANDARD_BYTES_252_FILE_SIZE = f_size(&arm_binary_STANDARD_BYTES_252_FILE);
	    arm_binary_STANDARD_BYTES_252 = (uint8_t*) pvPortMalloc(sizeof(uint8_t) * arm_binary_STANDARD_BYTES_252_FILE_SIZE);
	    if (arm_binary_STANDARD_BYTES_252 != NULL) {
	        f_read(&arm_binary_STANDARD_BYTES_252_FILE, arm_binary_STANDARD_BYTES_252, arm_binary_STANDARD_BYTES_252_FILE_SIZE, &br);
	    }
	    f_close(&arm_binary_STANDARD_BYTES_252_FILE);
	}
EOF

# 5. Remove trailing newlines from heredocs to match internal file format
chomp($replace1);
chomp($replace2);

# 6. Perform the substitution
# We look for leading whitespace (\s*) before the first line to clean that up too.
if ($content =~ s/\s*$pattern1/$replace1/s) {
    open my $out, '>', $output_path or die $!;
    print $out $content;
    close $out;
    print "Success! Cleanly replaced the block.\n";
} else {
    print "Pattern not found. Double-check for small typos in @find_lines1.\n";
}

if ($content =~ s/\s*$pattern2/$replace2/s) {
    open my $out, '>', $output_path or die $!;
    print $out $content;
    close $out;
    print "Success! Cleanly replaced the block.\n";
} else {
    print "Pattern not found. Double-check for small typos in @find_lines2.\n";
}



