int tcp_resize_buffer(struct tcp_attr *params, struct tcp_recived *rs)
{
	record rec;
	char *buffer_before;

	buffer_before = rs->buffer;
	rs->buffer_len += P_SIZE;
	rs->buffer = realloc(rs->buffer, rs->buffer_len);

	if(rs->buffer == NULL) 
	{
		rec.error = errno;
		REC_ERR(ERROR, rec.error, "realloc() wewnątrz"
			" tcp_resize_buffer() / port=%d, service=%d",
			params->port,
			params->service);

		if(buffer_before != NULL) free(buffer_before);

		return 1;
	}

	return 0;
}
