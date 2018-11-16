/*
 * Copyright (C) 2013-2017 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * This code is a complete clean re-write of the stress tool by
 * Colin Ian King <colin.king@canonical.com> and attempts to be
 * backwardly compatible with the stress tool by Amos Waterland
 * <apw@rossby.metr.ou.edu> but has more stress tests and more
 * functionality.
 *
 * Test RTC functionality code at pointer:
 * https://casper.berkeley.edu/svn/trunk/roach/sw/linux/Documentation/rtc.txt
 */

#include <linux/rtc.h>
#include "stress-ng.h"

static const char *opt_fstat_dir = "/dev";
static sigjmp_buf jmpbuf;

void stress_set_fstat_dir(const char *optarg)
{
	opt_fstat_dir = optarg;
}

static const char *blacklist[] = {
	"/dev/watchdog"
};

/*
 *  handle_fstat_sigalrm()
 *      catch SIGALRM
 */
static void MLOCKED handle_fstat_sigalrm(int dummy)
{
	(void)dummy;
	opt_do_run = false;

	siglongjmp(jmpbuf, 1);
}

/*
 *  do_not_stat()
 *	Check if file should not be stat'd
 */
static bool do_not_stat(const char *filename)
{
	size_t i;

	for (i = 0; i < SIZEOF_ARRAY(blacklist); i++) {
		if (!strncmp(filename, blacklist[i], strlen(blacklist[i])))
			return true;
	}
	return false;
}

/*
 *  stress_fstat()
 *	stress system with fstat
 */
int stress_fstat(
	uint64_t *const counter,
	const uint32_t instance,
	const uint64_t max_ops,
	const char *name)
{
	typedef struct dir_info {
		char	*path;
		bool	ignore;
		bool	noaccess;
		struct dir_info *next;
	} dir_info_t;

	DIR *dp;
	dir_info_t *di;
	static dir_info_t *dir_info;
	struct dirent *d;
	NOCLOBBER int ret = EXIT_FAILURE;
	bool stat_some;
	const uid_t euid = geteuid();

	(void)instance;

	if (stress_sighandler(name, SIGALRM, handle_fstat_sigalrm, NULL) < 0)
		return EXIT_FAILURE;
	if (sigsetjmp(jmpbuf, 0) != 0) {
		ret = EXIT_SUCCESS;
		goto free_cache;
	}

	if ((dp = opendir(opt_fstat_dir)) == NULL) {
		printf("%s: opendir on %s failed: errno=%d: (%s)\n",
			name, opt_fstat_dir, errno, strerror(errno));
		return EXIT_FAILURE;
	}

	/* Cache all the directory entries */
	while ((d = readdir(dp)) != NULL) {
		char path[PATH_MAX];

		if (!opt_do_run) {
			ret = EXIT_SUCCESS;
			(void)closedir(dp);
			goto free_cache;
		}

		snprintf(path, sizeof(path), "%s/%s", opt_fstat_dir, d->d_name);
		if (do_not_stat(path))
			continue;
		if ((di = calloc(1, sizeof(*di))) == NULL) {
			printf("%s: out of memory\n", name);
			(void)closedir(dp);
			goto free_cache;
		}
		if ((di->path = strdup(path)) == NULL) {
			printf("%s: out of memory\n", name);
			free(di);
			(void)closedir(dp);
			goto free_cache;
		}
		di->ignore = false;
		di->noaccess = false;
		di->next = dir_info;
		dir_info = di;
	}
	(void)closedir(dp);

	printf("Added by Zoran Stojsavljevic November 15th, 2018\n");
	struct rtc_time rtc_tm;
	unsigned long data, tmp;
	int i, retval, irqcount;

	do {
		stat_some = false;

		for (di = dir_info; opt_do_run && di; di = di->next) {
			int fd;
			struct stat buf;

			if (!opt_do_run || (max_ops && *counter >= max_ops))
				goto aborted;

			if (di->ignore)
				continue;

			if ((stat(di->path, &buf) < 0) &&
			    (errno != ENOMEM)) {
				di->ignore = true;
				continue;
			}
			if ((lstat(di->path, &buf) < 0) &&
			    (errno != ENOMEM)) {
				di->ignore = true;
				continue;
			}
			if (di->noaccess)
				continue;

			/*
			 *  Opening /dev files such as /dev/urandom
			 *  may block when running as root, so
			 *  avoid this.
			 */
			// usleep(300000);
			printf("Opening the /dev/rtc0 driver\n");
			if (!euid) {
				fd = open("/dev/rtc0", O_RDONLY); // | O_NONBLOCK, 0);
				if (fd < 0) {
					di->noaccess = true;
					printf("Opened fd is %d, /dev/rtc0 does not exist!\n", fd);
					// continue;
					return fd;
				}
				printf("Opened file handle is %d\n", fd); // Zoran

				//------------------------------------------------

				// printf("\n\t\t\tRTC Driver Test Example.\n\n");

				/* Turn on update interrupts (one per second) */
				retval = ioctl(fd, RTC_UIE_ON, 0);
				if (retval == -1) {
					if (errno == ENOTTY) {
						printf("\n...Update IRQs not supported.\n");
						goto test_READ;
					}
					perror("RTC_UIE_ON ioctl");
					exit(errno);
				}

#if 0
				printf("Counting 1 update (1/sec) interrupts from reading /dev/rtc0\n");
				// fflush(stderr);
				// for (i=1; i<6; i++) {
					/* This read will block */
					retval = read(fd, &data, sizeof(unsigned long));
					if (retval == -1) {
						perror("read");
						exit(errno);
					}
					// printf(" %d",i);
					// fflush(stderr);
					irqcount++;
				// }
#endif

				// printf("Again, from using select(2) on /dev/rtc:\n");
				// fflush(stderr);
				// for (i=1; i<6; i++) {
				// i = 0;
				// while (1) {
				//	i++;
					struct timeval tv = {0, 1};     /* 1 usec timeout on select */
					fd_set readfds;

					FD_ZERO(&readfds);
					FD_SET(fd, &readfds);
					/* The select will wait until an RTC interrupt happens. */
					retval = select(fd+1, &readfds, NULL, NULL, &tv);
					if (retval == -1) {
						perror("select");
						exit(errno);
					}
					/* This read won't block unlike the select-less case above. */
					retval = read(fd, &data, sizeof(unsigned long));
					if (retval == -1) {
						perror("read");
						exit(errno);
					}
					// printf("%d",i);
					// fflush(stderr);
					irqcount++;
				// }

				/* Turn off update interrupts */
				retval = ioctl(fd, RTC_UIE_OFF, 0);
				if (retval == -1) {
					perror("RTC_UIE_OFF ioctl");
					exit(errno);
				}

test_READ:
#if 0
				/* Read the RTC time/date */
				retval = ioctl(fd, RTC_RD_TIME, &rtc_tm);
				if (retval == -1) {
					perror("RTC_RD_TIME ioctl");
					exit(errno);
				}

				printf("\n\nCurrent RTC date/time is %d-%d-%d, %02d:%02d:%02d.\n",
				rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900,
				rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

				/* Set the alarm to 5 sec in the future, and check for rollover */
				rtc_tm.tm_sec += 5;
				if (rtc_tm.tm_sec >= 60) {
					rtc_tm.tm_sec %= 60;
					rtc_tm.tm_min++;
				}
				if (rtc_tm.tm_min == 60) {
					rtc_tm.tm_min = 0;
					rtc_tm.tm_hour++;
				}
				if (rtc_tm.tm_hour == 24) {
					rtc_tm.tm_hour = 0;
					retval = ioctl(fd, RTC_ALM_SET, &rtc_tm);
					if (retval == -1) {
						if (errno == ENOTTY) {
							printf("\n...Alarm IRQs not supported.\n");
							goto done;
						}
					}
					perror("RTC_ALM_SET ioctl");
					exit(errno);
				}

				/* Read the current alarm settings */
				retval = ioctl(fd, RTC_ALM_READ, &rtc_tm);
				if (retval == -1) {
					perror("RTC_ALM_READ ioctl");
					exit(errno);
				}

				printf("Alarm time now set to %02d:%02d:%02d.\n",
				rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

				/* Enable alarm interrupts */
				retval = ioctl(fd, RTC_AIE_ON, 0);
				if (retval == -1) {
					perror("RTC_AIE_ON ioctl");
					exit(errno);
				}

				printf("Waiting 5 seconds for alarm...\n");
				// fflush(stderr);
				/* This blocks until the alarm ring causes an interrupt */
				retval = read(fd, &data, sizeof(unsigned long));
				if (retval == -1) {
					perror("read");
					exit(errno);
				}
				irqcount++;
				printf("okay. Alarm rang.\n");

				/* Disable alarm interrupts */
				retval = ioctl(fd, RTC_AIE_OFF, 0);
				if (retval == -1) {
					perror("RTC_AIE_OFF ioctl");
					exit(errno);
				}

				//------------------------------------------------

#endif
done:
				printf("Closing file handle %d\n", fd); // Zoran
				(void)close(fd);
			}
			else printf("The effective user ID of the calling process EUID = %d/n", euid); // Zoran
			stat_some = true;
			(*counter)++;
		}
	} while (stat_some && opt_do_run && (!max_ops || *counter < max_ops));

aborted:
	ret = EXIT_SUCCESS;
free_cache:
	/* Free cache */
	for (di = dir_info; di; ) {
		dir_info_t *next = di->next;

		free(di->path);
		free(di);
		di = next;
	}

	return ret;
}
