#include <esp_partition.h> 

typedef std::vector <String> Keys_vector;

struct nvs_entry
{
  uint8_t  Ns ;         // Namespace ID
  uint8_t  Type ;       // Type of value
  uint8_t  Span ;       // Number of entries used for this item
  uint8_t  Rvs ;        // Reserved, should be 0xFF
  uint32_t CRC ;        // CRC
  char     Key[16] ;    // Key in Ascii
  uint64_t Data ;       // Data in entry 
} ;

struct nvs_page                                     // For nvs entries
{                                                   // 1 page is 4096 bytes
  uint32_t  State ;
  uint32_t  Seqnr ;
  
  uint32_t  Unused[5] ;
  uint32_t  CRC ;
  uint8_t   Bitmap[32] ;
  nvs_entry Entry[126] ;
} ;

// Common data
nvs_page                  buf ;
 
uint8_t FindNsID ( const esp_partition_t* nvs, const char* ns )
{
  esp_err_t                 result = ESP_OK ;                 // Result of reading partition
  uint32_t                  offset = 0 ;                      // Offset in nvs partition
  uint8_t                   i ;                               // Index in Entry 0..125
  uint8_t                   bm ;                              // Bitmap for an entry
  uint8_t                   res = 0xFF ;                      // Function result


  while ( offset < nvs->size )
  {
    result = esp_partition_read ( nvs, offset,                // Read 1 page in nvs partition
                                  &buf,
                                  sizeof(nvs_page) ) ;
    if ( result != ESP_OK )
    {
      Serial.printf ( "Error reading NVS!" ) ;
      break ;
    }
    i = 0 ;
    while ( i < 126 )
    {
      bm = ( buf.Bitmap[i/4] >> ( ( i % 4 ) * 2 ) ) & 0x03 ;  // Get bitmap for this entry
      if ( ( bm == 2 ) &&
           ( buf.Entry[i].Ns == 0 ) &&  
           ( strcmp ( ns, buf.Entry[i].Key ) == 0 ) )
      {
        res = buf.Entry[i].Data & 0xFF ;                      // Return the ID
        offset = nvs->size ;                                  // Stop outer loop as well
        break ;
      }
      else
      {
        if ( bm == 2 )
        {
          i += buf.Entry[i].Span ;                             // Next entry
        }
        else
        {
          i++ ;
        }
      }
    }
    offset += sizeof(nvs_page) ;                              // Prepare to read next page in nvs
  }
  return res ;
} 

Keys_vector EnumNvs(){
  //char Key[16];
  Keys_vector keys;
  
  esp_partition_iterator_t  pi ;                              
  const esp_partition_t*    nvs ;
  esp_err_t                 result = ESP_OK ;
  uint8_t                   pagenr = 0 ;                      // Page number in NVS
  uint8_t                   i ;                               // Index in Entry 0..125
  uint8_t                   bm ;                              // Bitmap for an entry
  uint32_t                  offset = 0 ;                      // Offset in nvs partition
  uint8_t                   namespace_ID ;
  char partname[] = "nvs";

  pi = esp_partition_find ( ESP_PARTITION_TYPE_DATA,          // Get partition iterator for
                            ESP_PARTITION_SUBTYPE_ANY,        // this partition
                            partname ) ;
  if ( pi )
  {
    nvs = esp_partition_get ( pi ) ;                          // Get partition struct
    esp_partition_iterator_release ( pi ) ;                   // Release the iterator
    Serial.printf ( "Partition %s found, %d bytes",
               partname,
               nvs->size ) ;
  }
  else
  {
    Serial.printf ( "Partition %s not found!", partname ) ;
    return keys;
  }
  namespace_ID = FindNsID ( nvs, "TicketMaster" ) ;             // Find ID of our namespace in NVS
  Serial.printf ( "Namespace ID of TicketMaster is %d",
             namespace_ID ) ;
  while ( offset < nvs->size )
  {
    result = esp_partition_read ( nvs, offset,                // Read 1 page in nvs partition
                                  &buf,
                                  sizeof(nvs_page) ) ;
    if ( result != ESP_OK )
    {
      Serial.printf ( "Error reading NVS!" ) ;
      return keys;
    }
    //dbgprint ( "" ) ;
    //dbgprint ( "Dump page %d", pagenr ) ;
    //dbgprint ( "State is %08X", buf.State ) ;
    //dbgprint ( "Seqnr is %08X", buf.Seqnr ) ;
    //dbgprint ( "CRC   is %08X", buf.CRC ) ;
    i = 0 ;
    while ( i < 126 )
    {
      bm = ( buf.Bitmap[i/4] >> ( ( i % 4 ) * 2 ) ) & 0x03 ;  // Get bitmap for this entry
      if ( bm == 2 )
      {
        if ( ( namespace_ID == 0xFF ) ||                      // Show all if ID = 0xFF
             ( buf.Entry[i].Ns == namespace_ID ) )            // otherwise just my namespace
        {
          //Serial.printf ( "Key %03d: %s", i,                       // Print entrynr
          //           buf.Entry[i].Key ) ;                     // Print the key
          keys.push_back(buf.Entry[i].Key);
        }
        i += buf.Entry[i].Span ;                              // Next entry
      }
      else
      {
        i++ ;
      }
    }
    offset += sizeof(nvs_page) ;                              // Prepare to read next page in nvs
    pagenr++ ;
  } 
  return keys;
}