/**
 *  \file main.cpp
 *
 *  \brief CLI for converting an identity tangle tree to a PL path
 *
 *
 *  \author Joe Starr
 *
 */

#include <cstdint>
#include <cstdio>
#include <ostream>

#include <comp_i2pp_buf_size.h>
#include <comp_itt_to_path.h>
#include <notation_plpath.h>
#include <notation_wptt.h>
#include <notation_defs.h>
#include <computation_defs.h>

#include <argparse.h>
#include <cstddef>
#include <string.h>
#include <string>
#include <iostream>

/**
 * \brief The number of characters each point in a PL path can contribute.
 *
 * Maximum digits (without loss of a precision) for a double component (x, y, z) of a point is 24.
 *Each of component of a point is followed by one of `,` or `\n`. There are three components in each
 *point. Each point can be the only member of a segments which is followed by a `\n`.
 *
 */
#define CHAR_CNT_FOR_PATH_POINTS    (((24 + 1) * 3) + 1)


/**
 *  \brief Count the number of instances of a character in a string.
 *
 *  \param str The string to search
 *  \param c The character to search for
 *
 *  \return The count
 *
 */
int count_char(const char *str, char c)
{
    int         count = 0;
    const char *end   = str + strlen(str);


    for (const char *p = str; p < end; p++)
    {
        if (c == *p)
        {
            count++;
        }
    }

    return count;
}

static const char *const usages[] = {
    "itt2plp ",
    "itt2plp -n <wptt>",
    NULL,
};

/**
 * \brief main calling routine.
 *
 * \param argc A count of arguments.
 * \param argv A list of void pointers to argument.
 * \return An exit code 0 indicating success or failure otherwise.
 */
int main(int argc, const char **argv)
{
    unsigned int ret_note    = 0;
    unsigned int ret_config  = 0;
    unsigned int ret_compute = 0;
    char *       note_wptt_c = NULL;

    /* Parse arguments */
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_STRING('n',
                   "notation",
                   &note_wptt_c,
                   "A WPTT for an identity tangle tree.",
                   NULL,
                   0, 0),
        OPT_END(),
    };
    struct argparse        argparse;

    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nConvert an identity tangle tree into a PL path.",
                      "\nConvert an identity tangle tree into a PL path.");

    (void)argparse_parse(&argparse, argc, argv);


    /* No notation was passed read one from standard in*/
    if (NULL == note_wptt_c)
    {
        std::string note_wptt;

        /*@@@NOTE: I'm not sure if this is really a great way to do this. It requires feeding of an
         * EOF symbol. */
        std::string temp;
        std::getline(std::cin, temp);
        note_wptt += temp;
        while (!std::cin.eof())
        {
            std::getline(std::cin, temp);
            note_wptt += "\n" + temp;
        }
        note_wptt_c = new char[note_wptt.size() + 1];
        strcpy(note_wptt_c, note_wptt.c_str());
    }


    /* Count the number of vertices in the tree*/
    int wptt_buff_size = 0;

    wptt_buff_size += count_char(note_wptt_c, '('); /* Each essential vertex starts with a (*/
    wptt_buff_size += count_char(note_wptt_c, '['); /* Each leaf vertex starts with a [*/
    wptt_buff_size += count_char(note_wptt_c, ' '); /* Each internal vertex of a rational tangle is
                                                     * separated by as space */
    wptt_buff_size += count_char(note_wptt_c, 'i'); /* @@@BUG: This required due to a bug in the
                                                     * core libraries */

    /* If the size is zero something has gone wrong*/
    if (0 == wptt_buff_size)
    {
        std::cout << "Not and identity tangle tree" << std::endl;
        return 1;
    }

    /* Create an appropriately sized WPTT notation*/
    note_wptt_node_t *      wptt_nodes    = new note_wptt_node_t[wptt_buff_size];
    note_wptt_node_buffer_t wptt_node_buf = { .buffer = wptt_nodes,
                                              .size = (uint16_t)wptt_buff_size,
                                              .idx  = 0 };
    note_wptt_t             wptt = { .root = NULL,
                                     .node_buffer = &wptt_node_buf,
                                     .label       = NOTE_WPTT_V4_LABEL_UNINIT };


    /*Decode the input string into the WPTT*/
    ret_note = note_wptt_decode(note_wptt_c, &wptt);

    if ((NOTE_DEFS_DECODE_SUCCESS != ret_note) ||
        (NOTE_WPTT_V4_LABEL_I != wptt.label) ||
        (NULL == wptt.root))
    {
        std::cout << "Not and identity tangle tree" << std::endl;
        return 1;
    }

    /* Compute the size of the PL buffer */
    comp_i2pp_buf_size_config_t buff_size_cfg = { .storage_write = NULL,
                                                  .itt           = &wptt };

    ret_config  = comp_i2pp_buf_size_config(&buff_size_cfg);
    ret_compute = comp_i2pp_buf_size_compute();

    const comp_i2pp_buf_size_result_t *res_i2pp = comp_i2pp_buf_size_result();

    if ((COMP_DEFS_CONFIG_SUCCESS != ret_config) ||
        (COMP_DEFS_CONFIG_SUCCESS != ret_compute) ||
        (NULL == res_i2pp))
    {
        std::cout << "Error in PL buffer sizing" << std::endl;
        return 1;
    }

    /* Create an appropriately sized PL path notation*/
    note_plpath_point_t *  points   = new note_plpath_point_t[res_i2pp->buff_size];
    note_plpath_segment_t *segments = new note_plpath_segment_t[res_i2pp->buff_size];

    note_plpath_point_buffer_t buff_point = { .buffer = points,
                                              .size = res_i2pp->buff_size,
                                              .idx  = 0 };

    note_plpath_segment_buffer_t buff_seg = { .buffer = segments,
                                              .size = res_i2pp->buff_size,
                                              .idx  = 0 };

    note_plpath_t plpath = { .segments = NULL,
                             .pnt_buff = &buff_point,
                             .seg_buff = &buff_seg, };

    /*Convert the WPTT into a PL path */
    comp_itt_to_path_config_t i2pp_cfg = { .storage_write = NULL,
                                           .itt      = &wptt,
                                           .out_path = &plpath };

    ret_config  = comp_itt_to_path_config(&i2pp_cfg);
    ret_compute = comp_itt_to_path_compute();

    const comp_itt_to_path_result_t *res_conv = comp_itt_to_path_result();

    if ((COMP_DEFS_CONFIG_SUCCESS != ret_config) ||
        (COMP_DEFS_CONFIG_SUCCESS != ret_compute))
    {
        std::cout << "Error in PL buffer sizing" << std::endl;
        return 1;
    }

    /* Encode the PL path notation as a string */
    int   plout_size = res_i2pp->buff_size * CHAR_CNT_FOR_PATH_POINTS;     /* The maximum */
    char *plout      = new char[plout_size];

    ret_note = note_plpath_encode(*res_conv->path, plout, plout_size);

    if (NOTE_DEFS_DECODE_SUCCESS != ret_note)
    {
        std::cout << "Not and identity tangle tree" << std::endl;
        return 1;
    }

    std::cout << plout;

    std::cout.flush();

    delete [] plout;
    delete [] wptt_nodes;
    delete [] points;
    delete [] segments;


    return 0;
}
