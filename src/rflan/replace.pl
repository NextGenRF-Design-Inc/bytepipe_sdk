#!/usr/bin/perl
use strict;
use warnings;
use File::Path qw(make_path);

# 1. Define the file path
my ($input_path, $output_path, $arm_bin_size, $stream_bin_size) = @ARGV;


# 2. Read the entire file into a single string
open my $fh, '<', $input_path or die "Could not open $input_path: $!";
my $content = do { local $/; <$fh> };
close $fh;

# 3. Define the exact text to find
my @find_lines0 = (
	'int32_t error_code = 0;',
);

my @find_lines1_before = (
    'uint8_t* stream_binary_STANDARD_BYTES_252;',
    'FILE *stream_binary_STANDARD_BYTES_252_FILE = fopen("stream_binary_STANDARD_BYTES_252.bin", "rb");',
    'fseek(stream_binary_STANDARD_BYTES_252_FILE, 0, SEEK_END);',
    'int64_t stream_binary_STANDARD_BYTES_252_FILE_SIZE = ftell(stream_binary_STANDARD_BYTES_252_FILE);',
    'fseek(stream_binary_STANDARD_BYTES_252_FILE, 0, SEEK_SET);',
    'stream_binary_STANDARD_BYTES_252 = (uint8_t *) malloc(stream_binary_STANDARD_BYTES_252_FILE_SIZE * sizeof(uint8_t));',
    'fread(stream_binary_STANDARD_BYTES_252, sizeof(uint8_t), stream_binary_STANDARD_BYTES_252_FILE_SIZE, stream_binary_STANDARD_BYTES_252_FILE);',
    'fclose(stream_binary_STANDARD_BYTES_252_FILE);',
);
my @find_lines1_after = (
    'ADI_HANDLE_ERROR(error_code, adrv9001Device_0);',
);

my @find_lines2_before = (
    'uint8_t* arm_binary_STANDARD_BYTES_252;',
    'FILE *arm_binary_STANDARD_BYTES_252_FILE = fopen("arm_binary_STANDARD_BYTES_252.bin", "rb");',
    'fseek(arm_binary_STANDARD_BYTES_252_FILE, 0, SEEK_END);',
    'int64_t arm_binary_STANDARD_BYTES_252_FILE_SIZE = ftell(arm_binary_STANDARD_BYTES_252_FILE);',
    'fseek(arm_binary_STANDARD_BYTES_252_FILE, 0, SEEK_SET);',
    'arm_binary_STANDARD_BYTES_252 = (uint8_t *) malloc(arm_binary_STANDARD_BYTES_252_FILE_SIZE * sizeof(uint8_t));',
    'fread(arm_binary_STANDARD_BYTES_252, sizeof(uint8_t), arm_binary_STANDARD_BYTES_252_FILE_SIZE, arm_binary_STANDARD_BYTES_252_FILE);',
    'fclose(arm_binary_STANDARD_BYTES_252_FILE);',
);
my @find_lines2_after = (
    'ADI_HANDLE_ERROR(error_code, adrv9001Device_0);',
);

# 3. Escape each line automatically and join with flexible whitespace
# quotemeta() adds the backslashes for you!

my $ws = '\s*\n?\s*';

my $pattern0 = join($ws, map { quotemeta($_) } @find_lines0);

my $pattern1 = join($ws, map { quotemeta($_) } @find_lines1_before)
             . $ws
             . 'error_code\s*=\s*adi_adrv9001_Stream_Image_Write\(adrv9001Device_0,\s*0,\s*stream_binary_STANDARD_BYTES_252,\s*\d+,\s*ADI_ADRV9001_ARM_SINGLE_SPI_WRITE_MODE_STANDARD_BYTES_252\);'
             . $ws
             . join($ws, map { quotemeta($_) } @find_lines1_after);

my $pattern2 = join($ws, map { quotemeta($_) } @find_lines2_before)
             . $ws
             . 'error_code\s*=\s*adi_adrv9001_arm_Image_Write\(adrv9001Device_0,\s*0,\s*arm_binary_STANDARD_BYTES_252,\s*\d+,\s*ADI_ADRV9001_ARM_SINGLE_SPI_WRITE_MODE_STANDARD_BYTES_252\);'
             . $ws
             . join($ws, map { quotemeta($_) } @find_lines2_after);
	
# 4. Define the multi-line replacement text
my $replace0 = <<"EOF";


	int32_t error_code = 0;
	adi_adrv9001_Device_t * adrv9001Device_0 = &Instance->Device;
EOF

my $replace1 = <<"EOF";
	
	
	FRESULT fr;
	UINT br; // Bytes read
	FIL stream_binary_STANDARD_BYTES_252_FILE;
	int64_t stream_binary_STANDARD_BYTES_252_FILE_SIZE;
	uint8_t* stream_binary_STANDARD_BYTES_252;
	fr = f_open(&stream_binary_STANDARD_BYTES_252_FILE, "stream_binary_STANDARD_BYTES_252.bin", FA_OPEN_EXISTING | FA_READ);
	if( fr == FR_OK )
	{
		stream_binary_STANDARD_BYTES_252_FILE_SIZE = f_size(&stream_binary_STANDARD_BYTES_252_FILE);
	    stream_binary_STANDARD_BYTES_252 = (uint8_t*) Instance->Malloc(sizeof(uint8_t) * stream_binary_STANDARD_BYTES_252_FILE_SIZE);
	    if (stream_binary_STANDARD_BYTES_252 != NULL) {
	        f_read(&stream_binary_STANDARD_BYTES_252_FILE, stream_binary_STANDARD_BYTES_252, stream_binary_STANDARD_BYTES_252_FILE_SIZE, &br);
	    }
	    f_close(&stream_binary_STANDARD_BYTES_252_FILE);
	}

	error_code = adi_adrv9001_Stream_Image_Write(adrv9001Device_0, 0, stream_binary_STANDARD_BYTES_252, stream_binary_STANDARD_BYTES_252_FILE_SIZE, ADI_ADRV9001_ARM_SINGLE_SPI_WRITE_MODE_STANDARD_BYTES_252);
	ADI_HANDLE_ERROR(error_code, adrv9001Device_0);
	Instance->Free(stream_binary_STANDARD_BYTES_252);
EOF

my $replace2 = <<"EOF";
	
	
	FIL arm_binary_STANDARD_BYTES_252_FILE;
	int64_t arm_binary_STANDARD_BYTES_252_FILE_SIZE;
	uint8_t* arm_binary_STANDARD_BYTES_252;
	fr = f_open(&arm_binary_STANDARD_BYTES_252_FILE, "arm_binary_STANDARD_BYTES_252.bin", FA_OPEN_EXISTING | FA_READ);
	if( fr == FR_OK )
	{
		arm_binary_STANDARD_BYTES_252_FILE_SIZE = f_size(&arm_binary_STANDARD_BYTES_252_FILE);
	    arm_binary_STANDARD_BYTES_252 = (uint8_t*) Instance->Malloc(sizeof(uint8_t) * arm_binary_STANDARD_BYTES_252_FILE_SIZE);
	    if (arm_binary_STANDARD_BYTES_252 != NULL) {
	        f_read(&arm_binary_STANDARD_BYTES_252_FILE, arm_binary_STANDARD_BYTES_252, arm_binary_STANDARD_BYTES_252_FILE_SIZE, &br);
	    }
	    f_close(&arm_binary_STANDARD_BYTES_252_FILE);
	}

	error_code = adi_adrv9001_arm_Image_Write(adrv9001Device_0, 0, arm_binary_STANDARD_BYTES_252, arm_binary_STANDARD_BYTES_252_FILE_SIZE, ADI_ADRV9001_ARM_SINGLE_SPI_WRITE_MODE_STANDARD_BYTES_252);
	ADI_HANDLE_ERROR(error_code, adrv9001Device_0);
	Instance->Free(arm_binary_STANDARD_BYTES_252);
EOF

# 5. Remove trailing newlines from heredocs to match internal file format
chomp($replace1);
chomp($replace2);

# 6. Perform the substitution
# We look for leading whitespace (\s*) before the first line to clean that up too.
if ($content =~ s/\s*$pattern0/$replace0/s) {
    open my $out, '>', $output_path or die $!;
    print $out $content;
    close $out;
    print "Success! Cleanly replaced the block.\n";
} else {
    print "Pattern not found. Double-check for small typos in @find_lines0.\n";
}

if ($content =~ s/\s*$pattern1/$replace1/s) {
    open my $out, '>', $output_path or die $!;
    print $out $content;
    close $out;
    print "Success! Cleanly replaced the block.\n";
} else {
    print "Pattern not found. Double-check for small typos in @find_lines1_after and @find_lines1_before.\n";
}

if ($content =~ s/\s*$pattern2/$replace2/s) {
    open my $out, '>', $output_path or die $!;
    print $out $content;
    close $out;
    print "Success! Cleanly replaced the block.\n";
} else {
    print "Pattern not found. Double-check for small typos in @find_lines2_after and @find_lines2_before.\n";
}



