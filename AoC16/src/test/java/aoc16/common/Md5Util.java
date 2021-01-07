package aoc16.common;

import java.security.MessageDigest;

public class Md5Util {
    final MessageDigest messageDigest;

    public Md5Util() {
        try {
            messageDigest = MessageDigest.getInstance("MD5");
        } catch ( Exception e ) {
            throw new RuntimeException( "MD5 not found", e );
        }
    }

    public String md5(String text) {
        messageDigest.update(text.getBytes());
        final var sb = new StringBuilder();
        for ( final var b : messageDigest.digest() ) {
            sb.append( String.format("%02X", b) );
        }
        return sb.toString();
    }
}
